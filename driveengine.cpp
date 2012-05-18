#include "driveengine.h"
#include <QDebug>
#include <QSettings>
#include "AppRegData.h"
#include <QMessageBox>

DriveEngine::DriveEngine(QObject *parentObj) :
    QObject(parentObj),
    networkAccessManager(NULL),
    parent(static_cast<QWidget*>(parentObj)),
    model(NULL),
    parser(NULL),
    reader(NULL),
    oAuth2(NULL),
    downloadManager(NULL),
    uploadFileManager(NULL),
    filesManager(NULL)
{
}

DriveEngine::~DriveEngine()
{
    if(networkAccessManager) delete networkAccessManager;
    if(downloadManager) delete downloadManager;
    if(uploadFileManager) delete uploadFileManager;
    if(model) delete model;
    if(parser) delete parser;
    if(oAuth2) delete oAuth2;
}

void DriveEngine::slotStartLogin(void)
{
    oAuth2->startLogin(true);
}

void DriveEngine::init(void)
{
    if(networkAccessManager) delete networkAccessManager;
    networkAccessManager = new QNetworkAccessManager(this);

    if(oAuth2) delete oAuth2;
    oAuth2 = new OAuth2(parent);

    setConnections();
}

void DriveEngine::setConnections(void)
{
    connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotReplyFinished(QNetworkReply*)));
    connect(parent, SIGNAL(siganalGet()), this, SLOT(slotGet()));
    connect(UiInstance::ui->discTreeView, SIGNAL(expanded(const QModelIndex&)), this, SLOT(slotTreeViewExpanded(const QModelIndex&)));
    connect(UiInstance::ui->discTreeView, SIGNAL(collapsed(const QModelIndex&)), this, SLOT(slotTreeViewCollapsed(const QModelIndex&)));
    connect(UiInstance::ui->discTreeView, SIGNAL(clicked (const QModelIndex&)), this, SLOT(slotTreeViewClicked(const QModelIndex&)));
    //connect(UiInstance::ui->filesViewWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slotItemClicked(QTreeWidgetItem*, int)));
}

void DriveEngine::slotReplyFinished(QNetworkReply* reply)
{
    if(parseReply(replyStr, FOLDER_TYPE)) setModel();
}

void DriveEngine::setModel(void)
{
    if(model) delete model;

    QList<QVariant> rootData;

    rootData << TREE_VIEW_MAIN_TITLE;

    TreeItemInfo* itemInfo = parser->getXMLHandler()->getTreeItemInfo();

    model = new TreeModel(rootData, itemInfo);
    UiInstance::ui->discTreeView->setModel(model);
    UiInstance::ui->filesViewWidget->header()->resizeSection(0, 400);
}

void DriveEngine::slotGet(void)
{
    CommonTools::setHeader(request);
    request.setUrl(QUrl(GET_FOLDERS));
    reply = networkAccessManager->get(request);
    settings();
}

void DriveEngine::slotFoldersReadyRead()
{
    //qDebug() << "slotFoldersReadyRead";
    replyStr.append(reply->readAll());
}

void DriveEngine::slotFoldersError(QNetworkReply::NetworkError error)
{
    qDebug() << "slotFoldersError error:" << error;

    if(error == QNetworkReply::QNetworkReply::UnknownNetworkError)
       qDebug() << "\n*******************\nIf this error occur, please make sure that you have openssl installed (also you can try just copy libeay32.dll and ssleay32.dll files from Qt SDK QtCreator/bin folder into your folder where your program .exe file located (tested on non-static compilation only))\n*******************\n";

    if(error == QNetworkReply::AuthenticationRequiredError) emit signalAccessTokenExpired();
}

void DriveEngine::slotFoldersSslErrors(const QList<QSslError>& errors)
{
    foreach(const QSslError& e,errors)
    {
        qDebug() << "Ssl error:" << e.error();
    }
}

void DriveEngine::settings(void)
{
    connect(reply, SIGNAL(readyRead()), this, SLOT(slotFoldersReadyRead()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(slotFoldersError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(sslErrors(const QList<QSslError>&)),this, SLOT(slotFoldersSslErrors(const QList<QSslError>&)));
}

bool DriveEngine::parseReply(const QString& str, int type)
{
    QXmlInputSource source;

    if(parser) delete parser;
    if(reader) delete reader;
    parser = new XMLParser(type);
    reader = new QXmlSimpleReader;

    connect(parser->getXMLHandler(), SIGNAL(signalAllResDownloaded(int)),this, SLOT(slotResDownloaded(int)));
    source.setData(str.toAscii());
    reader->setContentHandler(parser);
    reader->setErrorHandler(parser);

    return reader->parse(&source);
}

void DriveEngine::slotResDownloaded(int queryType)
{
  if(queryType == FOLDER_TYPE) UiInstance::ui->discTreeView->collapseAll();
}

OAuth2* DriveEngine::getOAuth2(void) const
{
    return oAuth2;
}

void DriveEngine::slotDownload(void)
{
    if(downloadManager)
    {
        if(downloadManager->getState() == NetworkManager::EBusy) return;
    }

    QSettings settings(COMPANY_NAME, APP_NAME);

    QList<TreeItemInfo::Data> treeItems = filesManager->getParser()->getXMLHandler()->getTreeItemInfo()->getFileItems();
    int index = getCurrentFileItemIndex();

    QString downloadLink(treeItems[index].downloadLink);

    if(!downloadLink.isEmpty())
    {
        if(slotCheckWorkDir(false))
        {
            QString fileName = settings.value(WORK_DIR).toString() + "\/" + treeItems[index].name;
            QString fileType =  treeItems[index].fileType;

            if(downloadManager) delete downloadManager;
            downloadManager = new DownloadFileManager(parent);

            downloadManager->startDownload(QUrl(downloadLink), fileName, fileType);
        }
        else CommonTools::msg(SET_DIR_REMINDER_MSG);
    }
}

void DriveEngine::slotUpload(void)
{
    if(uploadFileManager)
    {
        if(uploadFileManager->getState() == NetworkManager::EBusy) return;
    }

    //qDebug() << "index.row():"  << QString::number(currentFoderIndex.row());

    QSettings settings(COMPANY_NAME, APP_NAME);
    accessToken = settings.value("access_token").toString();

    QString fileName = QFileDialog::getOpenFileName(parent, "Uploading file", QDir::homePath(), "All files(*)");

    if(!fileName.isEmpty())
    {
        TreeItemInfo treeItems = *parser->getXMLHandler()->getTreeItemInfo();
        int index = getCurrentModelItemIndex();

        QString uploadLink(treeItems[index].uploadLink + "/?convert=false");

        if(!uploadLink.isEmpty())
        {
            if(uploadFileManager) delete uploadFileManager;
            uploadFileManager = new UploadFileManager(parent);

            connect(uploadFileManager, SIGNAL(signalUpdateFileList()), parent, SLOT(slotUpdateFileList()));

            uploadFileManager->startUpload(uploadLink, fileName);
        }
    }
}

int DriveEngine::getCurrentModelItemIndex(void) const
{
    TreeItem *item = static_cast<TreeItem*>(UiInstance::ui->discTreeView->currentIndex().internalPointer());
    TreeItemInfo treeItems = *parser->getXMLHandler()->getTreeItemInfo();
    int count = treeItems.getItems().count();

    int currentModelIndex = 0;

    for(int i = 1; i < count; ++i)
    {
        if(treeItems[i].item == item)
        {
            currentModelIndex = i;
            break;
        }
    }

    return currentModelIndex;
}

int DriveEngine::getCurrentFileItemIndex(void) const
{
    QList<TreeItemInfo::Data> fileItemsInfo = filesManager->getParser()->getXMLHandler()->getTreeItemInfo()->getFileItems();
    int count = fileItemsInfo.count();
    QString fileName(UiInstance::ui->filesViewWidget->currentIndex().data().toString());

    int currentFileIndex = 0;

    for(int i = 1; i < count; ++i)
    {
        if(fileName == fileItemsInfo[i].name)
        {
            currentFileIndex = i;
            break;
        }
    }

    return currentFileIndex;
}

bool DriveEngine::slotCheckWorkDir(bool showDlg)
{
    QSettings settings(COMPANY_NAME, APP_NAME);
    SettingsDialog dlg(parent);
    bool dirTextNotEmpty = false;

    if(settings.contains(WORK_DIR) && showDlg)
    {
        dlg.setDirectoryPath(settings.value(WORK_DIR).toString());
    }
    else if(settings.contains(WORK_DIR) && !showDlg)
    {
        return true;
    }

    switch(dlg.exec())
    {
    case QDialog::Accepted:
    {
        if(!dlg.directoryPath().isEmpty() )
        {
            settings.setValue(WORK_DIR,dlg.directoryPath());
            dirTextNotEmpty = true;
        }

    }
        break;
    }

    return dirTextNotEmpty;
}

void DriveEngine::slotTreeViewExpanded(const QModelIndex& index)
{
    CommonTools::addTreeViewOpenedItem(index.row());
}

void DriveEngine::slotTreeViewCollapsed(const QModelIndex& index)
{
    CommonTools::removeTreeViewOpenedItem(index.row());
}

void DriveEngine::slotTreeViewClicked(const QModelIndex& index)
{
  showFiles();
}

//void DriveEngine::slotItemClicked(QTreeWidgetItem* item, int column)
//{
//    qDebug() << "column:"  << QString::number(column);
//}

void DriveEngine::showFiles(void)
{
    TreeItemInfo treeItems = *parser->getXMLHandler()->getTreeItemInfo();
    int treeItemsIndex = getCurrentModelItemIndex();

    if(treeItems[treeItemsIndex].type == FOLDER_TYPE_STR)
    {
       if(!filesManager) filesManager = new FilesManager;

       QString query(treeItems[treeItemsIndex].self);
       query += QString("/contents");

       filesManager->getFiles(query);
    }
}

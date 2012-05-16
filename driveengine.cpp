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
    uploadFileManager(NULL)
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
}

void DriveEngine::slotReplyFinished(QNetworkReply* reply)
{
    if(!replyStr[EFolders].isEmpty() && !replyStr[EFiles].isEmpty())
    {
//        CommonTools::logToFile("Folders.txt", replyStr[EFolders].toAscii());
//        CommonTools::logToFile("Files.txt", replyStr[EFiles].toAscii());

        if(!parseReply(replyStr[EFolders], FOLDER_TYPE)) qDebug() << "parseReply(replyStr[EFolders] NOT OK";

        if(parseReply(replyStr[EFiles], FILE_TYPE)) setModel();
        else qDebug() << "parseReply(replyStr[EFiles] NOT OK";

        //parseReply(CommonTools::loadFromFile("folder.tre"), FOLDER_TYPE);
        //if(parseReply(CommonTools::loadFromFile("files.tre"), FILE_TYPE)) setModel();
    }
}

void DriveEngine::setModel(void)
{
    if(model) delete model;

    QList<QVariant> rootData;

    /*
      add other columns into rootData here, if necessary (i.e. columns with titles which contain additional info about items)
      (example: rootData << TREE_VIEW_MAIN_TITLE << OTHER_COLIMN_TITLE1 <<  OTHER_COLIMN_TITLE2;)
    */

    rootData << TREE_VIEW_MAIN_TITLE << TREE_VIEW_UPDATED_TITLE << TREE_VIEW_SIZE_TITLE;
    //rootData << TREE_VIEW_MAIN_TITLE << "parent" << "self" << TREE_VIEW_UPDATED_TITLE << TREE_VIEW_SIZE_TITLE;

    TreeItemInfo* itemInfo = parser->getXMLHandler()->getTreeItemInfo();

    itemInfo->normalize();

    model = new TreeModel(rootData, itemInfo);
    UiInstance::ui->discTreeView->setModel(model);

    loadOpenedItems();

    for(int i = 1; i < model->columnCount(); ++i)
        UiInstance::ui->discTreeView->header()->resizeSection(i, 120);

    UiInstance::ui->discTreeView->header()->resizeSection(0, 750);

    //qDebug() << " UiInstance::ui->discTreeView->collapseAll();";
}

void DriveEngine::slotGet(void)
{
    QStringList requestStr;

    requestStr << GET_FOLDERS;
    requestStr << GET_FILES;

    for(int i = EFolders;i < ERepliesCount;++i)
    {
        CommonTools::setHeader(request[i]);
        request[i].setUrl(QUrl(requestStr[i]));
        reply[i] = networkAccessManager->get(request[i]);
        settings(static_cast<EReplies> (i));
    }
}

void DriveEngine::slotFoldersReadyRead()
{
    qDebug() << "slotFoldersReadyRead";
    replyStr[EFolders].append(reply[EFolders]->readAll());
}

void DriveEngine::slotFoldersError(QNetworkReply::NetworkError error)
{
    qDebug() << "slotFoldersError error = " << error;
}

void DriveEngine::slotFoldersSslErrors(const QList<QSslError>& errors)
{
    qDebug() << "slotFoldersSslErrors error";

    foreach(const QSslError& e,errors)
    {
        qDebug() << "error = " << e.error();
    }
}

void DriveEngine::slotFilesReadyRead()
{
    qDebug() << "slotFilesReadyRead";
    replyStr[EFiles].append(reply[EFiles]->readAll());
}

void DriveEngine::slotFilesError(QNetworkReply::NetworkError error)
{
    qDebug() << "slotFilesError error = " << error;
    if(error == QNetworkReply::AuthenticationRequiredError) emit signalAccessTokenExpired();
}

void DriveEngine::slotFilesSslErrors(const QList<QSslError>& errors)
{
    qDebug() << "slotFilesSslErrors error";

    foreach(const QSslError& e,errors)
    {
        qDebug() << "error = " << e.error();
    }
}

void DriveEngine::settings(EReplies eReply)
{
    switch(eReply)
    {
    case EFolders:
    {
        connect(reply[EFolders], SIGNAL(readyRead()), this, SLOT(slotFoldersReadyRead()));
        connect(reply[EFolders], SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(slotFoldersError(QNetworkReply::NetworkError)));
        connect(reply[EFolders], SIGNAL(sslErrors(const QList<QSslError>&)),this, SLOT(slotFoldersSslErrors(const QList<QSslError>&)));
    }
        break;
    case EFiles:
    {
        connect(reply[EFiles], SIGNAL(readyRead()), this, SLOT(slotFilesReadyRead()));
        connect(reply[EFiles], SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(slotFilesError(QNetworkReply::NetworkError)));
        connect(reply[EFiles], SIGNAL(sslErrors(const QList<QSslError>&)),this, SLOT(slotFilesSslErrors(const QList<QSslError>&)));
    }
        break;
    }
}

bool DriveEngine::parseReply(const QString& str, int type)
{
    QXmlInputSource source;

    if(type == FOLDER_TYPE)
    {
        if(parser) delete parser;
        if(reader) delete reader;
        parser = new XMLParser(type);
        reader = new QXmlSimpleReader;
    }
    else
    {
        parser->setType(type);
    }

    source.setData(str.toAscii());

    reader->setContentHandler(parser);
    reader->setErrorHandler(parser);

    return reader->parse(&source);
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
    TreeItemInfo treeItems = *parser->getXMLHandler()->getTreeItemInfo();
    int index = getCurrentModelItemIndex();
    QString downloadLink(treeItems[index].downloadLink);

    if(!downloadLink.isEmpty())
    {
        if(slotCheckWorkDir(false))
        {
            TreeItemInfo treeItems = *parser->getXMLHandler()->getTreeItemInfo();
            int index = getCurrentModelItemIndex();

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

            connect(uploadFileManager, SIGNAL(signalUpdateModel()), parent, SLOT(slotUpdateModel()));

            uploadFileManager->startUpload(uploadLink, fileName);
        }
    }
}

int DriveEngine::getCurrentModelItemIndex(void) const
{
    TreeItem *item = static_cast<TreeItem*>(UiInstance::ui->discTreeView->currentIndex().internalPointer());
    TreeItemInfo treeItems = *parser->getXMLHandler()->getTreeItemInfo();
    int count = parser->getXMLHandler()->getTreeItemInfo()->getItems().count();

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

void DriveEngine::loadOpenedItems(void)
{
    QList<int> indexes = CommonTools::getTreeViewOpenedItem();
    CommonTools::treeViewOpenedItemClear();

    for(int i = 0; i < indexes.count(); ++i)
    {
        UiInstance::ui->discTreeView->expand(model->index(indexes[i], 0, QModelIndex()));
    }
}

void DriveEngine::slotTreeViewExpanded(const QModelIndex& index)
{
    CommonTools::addTreeViewOpenedItem(index.row());
}

void DriveEngine::slotTreeViewCollapsed(const QModelIndex& index)
{
    CommonTools::removeTreeViewOpenedItem(index.row());
}

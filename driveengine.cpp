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

    UiInstance::ui->actionMenuDownload->setEnabled(true);
    UiInstance::ui->actionDownload->setEnabled(true);
}

void DriveEngine::init(void)
{
    if(networkAccessManager) delete networkAccessManager;
    networkAccessManager = new QNetworkAccessManager(this);

    if(oAuth2) delete oAuth2;
    oAuth2 = new OAuth2;

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
    //    qDebug() << "--------------> replyStr[EFolders]" << replyStr[EFolders];
    //    qDebug() << "--------------> replyStr[EFiles]" << replyStr[EFiles];

    if(!replyStr[EFolders].isEmpty() && !replyStr[EFiles].isEmpty())
    {
        if(!parseReply(replyStr[EFolders], FOLDER_TYPE)) qDebug() << "parseReply(replyStr[EFolders] NOT OK";

        if(parseReply(replyStr[EFiles], FILE_TYPE)) setModel();
        else qDebug() << "parseReply(replyStr[EFiles] NOT OK";
    }
}

void DriveEngine::setModel(void)
{
    if(model)
    {
        delete model;
        model = NULL;
    }

    QList<QVariant> rootData;

    /*
      add other columns into rootData here, if necessary (i.e. columns with titles which contain additional info about items)
      (example: rootData << TREE_VIEW_MAIN_TITLE << OTHER_COLIMN_TITLE1 <<  OTHER_COLIMN_TITLE2;)
    */

    //rootData << TREE_VIEW_MAIN_TITLE;

    rootData << TREE_VIEW_MAIN_TITLE << TREE_VIEW_PUBLISHED_TITLE << TREE_VIEW_UPDATED_TITLE << TREE_VIEW_EDITED_TITLE << TREE_VIEW_SIZE_TITLE;

    TreeItemInfo* itemInfo = parser->getXMLHandler()->getTreeItemInfo();

    itemInfo->normalize();

    model = new TreeModel(rootData, itemInfo);
    UiInstance::ui->discTreeView->setModel(model);
    slotAdditionalInfoCheckBox(UiInstance::ui->actionAdditionalInfo->isChecked());
    loadOpenedItems();
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
    //    if(downloadManager)
    //    {
    //        if(downloadManager->getState() == DownloadFileManager::EBusy)
    //        {
    //            CommonTools::msg("No multithreading support for files downloading in this version");
    //            return;
    //        }
    //    }

    UiInstance::ui->actionMenuDownload->setDisabled(true);
    UiInstance::ui->actionDownload->setDisabled(true);

    QSettings settings(COMPANY_NAME, APP_NAME);
    QString downloadLink(parser->getXMLHandler()->getTreeItemInfo()->getItems()[getCurrentModelItemIndex()].downloadLink);

    if(!downloadLink.isEmpty())
    {
        if(slotCheckWorkDir(false))
        {
            QString fileName = settings.value(WORK_DIR).toString() + "\/" + parser->getXMLHandler()->getTreeItemInfo()->getItems()[getCurrentModelItemIndex()].name.toString();
            QString fileType =  parser->getXMLHandler()->getTreeItemInfo()->getItems()[getCurrentModelItemIndex()].fileType;

            if(downloadManager) delete downloadManager;
            downloadManager = new DownloadFileManager;

            downloadManager->startDownload(QUrl(downloadLink), fileName, fileType);
        }
        else CommonTools::msg("Please note: you must set working directory for downloading files");
    }
}

void DriveEngine::slotUpload(void)
{
    qDebug() << "slotUpload ";

    QSettings settings(COMPANY_NAME, APP_NAME);
    accessToken = settings.value("access_token").toString();

    QString fileName = QFileDialog::getOpenFileName(parent, trUtf8("Uploading file"), QDir::homePath(), trUtf8("All files(*)"));

    if(!fileName.isEmpty())
    {
        UiInstance::ui->actionMenuUpload->setDisabled(true);
        UiInstance::ui->actionUpload->setDisabled(true);

        QString uploadLink(parser->getXMLHandler()->getTreeItemInfo()->getItems()[getCurrentModelItemIndex()].uploadLink);

        if(!uploadLink.isEmpty())
        {
            if(uploadFileManager) delete uploadFileManager;
            uploadFileManager = new UploadFileManager;

            connect(uploadFileManager, SIGNAL(signalUpdateModel()), this, SLOT(slotUploadFinished()));

            uploadFileManager->startUpload(fileName, uploadLink, accessToken);
        }
    }
}

void DriveEngine::slotUploadFinished()
{
    qDebug() << "DriveEngine::slotUploadFinished";
    emit signalUploadFinished();
}

int DriveEngine::getCurrentModelItemIndex(void) const
{
    TreeItem *item = static_cast<TreeItem*>(UiInstance::ui->discTreeView->currentIndex().internalPointer());
    int count = parser->getXMLHandler()->getTreeItemInfo()->getItems().count();
    int currentModelIndex = 0;

    for(int i = 1; i < count; ++i)
    {
        if(parser->getXMLHandler()->getTreeItemInfo()->getItems()[i].item == item)
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

   //qDebug() << "indexes.count() --------------------------->" << QString::number(indexes.count());

    for(int i = 0; i < indexes.count(); ++i)
    {
        //qDebug() << "indexes.count() ---------------------------> indexes[" << i << "]=" << QString::number(indexes[i]);
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

void DriveEngine::slotAdditionalInfoCheckBox(bool state)
{
   qDebug() << "additionalInfoCheckBox " << state;

   for(int i = 1; i < model->getColumnCount(); ++i)
   {
       UiInstance::ui->discTreeView->setColumnHidden(i,!state);
       UiInstance::ui->discTreeView->header()->resizeSection(i, 180);
   }

   //UiInstance::ui->discTreeView->header()->resizeSections(QHeaderView::ResizeToContents);
   UiInstance::ui->discTreeView->header()->resizeSection(0, 250);
}


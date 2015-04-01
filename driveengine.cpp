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
    downloadManager(NULL)
{
}

DriveEngine::~DriveEngine()
{
    if(networkAccessManager) delete networkAccessManager;
    if(downloadManager) delete downloadManager;
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
    oAuth2 = new OAuth2;

    connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotReplyFinished(QNetworkReply*)));
    connect(parent, SIGNAL(siganalGet()), this, SLOT(slotGet()));
    connect(parent, SIGNAL(signalPost()), this, SLOT(slotPost()));
}

void DriveEngine::slotReplyFinished(QNetworkReply* reply)
{
//    qDebug() << "--------------> replyStr[EFolders]" << replyStr[EFolders];
//    qDebug() << "--------------> replyStr[EFiles]" << replyStr[EFiles];

    if(replyStr[EFolders] != "" &&replyStr[EFiles] != "" )
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

    rootData << TREE_VIEW_MAIN_TITLE;
    //rootData << TREE_VIEW_MAIN_TITLE << "test1" << "test2";

    TreeItemInfo* itemInfo = parser->getXMLHandler()->getTreeItemInfo();

    itemInfo->normalize();

    model = new TreeModel(rootData, itemInfo);
    UiInstance::ui->discTreeView->setModel(model);
}

void DriveEngine::slotGet(void)
{
    QStringList requestStr;

    requestStr << GET_FOLDERS;
    requestStr << GET_FILES;

    for(int i = EFolders;i < ECount;++i)
    {
        setHeader(request[i]);
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
}

void DriveEngine::slotFilesSslErrors(const QList<QSslError>& errors)
{
    qDebug() << "slotFilesSslErrors error";

    foreach(const QSslError& e,errors)
    {
        qDebug() << "error = " << e.error();
    }
}

void DriveEngine::slotPost(void)
{
    qDebug() << "slotPost";

    //    setHeader();

    //    QString postQuery = "https://www.googleapis.com/drive/v1/files";

    //    request.setUrl(QUrl(postQuery));

    //    QByteArray data;
    //    data.append("&title=pets");
    //    data.append("&mimeType=application/vnd.google-apps.folder");
    //    QUrl dataPart;
    //    dataPart.setEncodedUrl(data);

    //    reply = networkAccessManager->post(request, data);
    //    qDebug() << "URL = " << postQuery + QString(data);

    //    settings();
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

void DriveEngine::setHeader(QNetworkRequest& request)
{
    QSettings settings(COMPANY_NAME, APP_NAME);
    accessToken = settings.value("access_token").toString();

    request.setRawHeader("User-Agent", "Google Drive client");
    request.setRawHeader("GData-Version", "3.0");
    request.setRawHeader("Authorization",(QString("OAuth %1").arg(accessToken)).toLatin1());
}

bool DriveEngine::parseReply(const QString& str, int type)
{
    //QXmlSimpleReader reader;
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

#if QT_VERSION >= 0x050000
    // Qt5 code
    source.setData(str.toUtf8());
#else
    // Qt4 code
    source.setData(str.toLatin1());
#endif




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
    QSettings settings(COMPANY_NAME, APP_NAME);
    QString link(parser->getXMLHandler()->getTreeItemInfo()->getItems()[getCurrentModelItemIndex()].downloadLink);

    if(link != "")
    {
        if(slotCheckWorkDir(false))
        {
            QString filePath = settings.value(WORK_DIR).toString() + "\/" +parser->getXMLHandler()->getTreeItemInfo()->getItems()[getCurrentModelItemIndex()].name.toString();

            if(downloadManager) delete downloadManager;
            downloadManager = new DownloadFileManager;

            downloadManager->startDownload(QUrl(link), filePath);
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("Please set working directory for downloading files");
            msgBox.exec();
        }
    }
}

int DriveEngine::getCurrentModelItemIndex(void) const
{
    TreeItem *item = static_cast<TreeItem*>(UiInstance::ui->discTreeView->currentIndex().internalPointer());
    int currentModelIndex = 0;

    for(int i = 1; i < parser->getXMLHandler()->getTreeItemInfo()->getItems().count(); ++i)
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
        qDebug() << "QDialog::Accepted";
        if(dlg.directoryPath() != "" )
        {
            settings.setValue(WORK_DIR,dlg.directoryPath());
            dirTextNotEmpty = true;
        }

    }
        break;
    }

    return dirTextNotEmpty;
}


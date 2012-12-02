#include "network/operationsmanager.h"
#include "share/commontools.h"
#include "core/driveengine.h"
#include "share/debug.h"
#include <QStringList>

OperationsManager::OperationsManager(QObject *parent):
    NetworkManager(parent)
{
}

void OperationsManager::deleteFile(const QString &url)
{
    CommonTools::setHeader(request);
    request.setRawHeader("If-Match", "*");

    init();

    request.setUrl(getDeleteFileQuery(url));

    DEBUG << getDeleteFileQuery(url);

    reply = networkManager->deleteResource(request);

    connect(networkManager.data(), SIGNAL(finished(QNetworkReply*)),this, SLOT(slotReplyFinished(QNetworkReply*)));
    connectErrorHandlers();
}

void OperationsManager::copyWebFile(const QString &source, const QString &dest)
{
    DEBUG << "source" << source << "dest" << dest;

    //SDriveEngine::inst()->getfilesTransferUI()->upload(source);

    //    QString uploadLink(dest + "/?convert=false");

    //    //SDriveEngine::inst()->uploadFileMngr.reset(new UploadFileManager(SDriveEngine::inst()->parent));
    //  //  connect(SDriveEngine::inst()->uploadFileMngr.data(), SIGNAL(signalUpdateFileList()), SDriveEngine::inst()->filesUI.data(), SLOT(slotUpdateFileList()));
    //    SDriveEngine::inst()->uploadFileMngr->startUpload(uploadLink, source);


    //    QString uploadLink(source + "/?convert=false");

    //    SDriveEngine::inst()->uploadFileMngr.reset(new UploadFileManager(SDriveEngine::inst()->parent));
    //    connect(SDriveEngine::inst()->uploadFileMngr.data(), SIGNAL(signalUpdateFileList()), SDriveEngine::inst()->filesUI.data(), SLOT(slotUpdateFileList()));
    //    SDriveEngine::inst()->uploadFileMngr->startUpload(uploadLink, fileName);


    //    QUrl fileURL(getCopyFileQuery(source));
    //    QString src(source+ "/?convert=false");



    //    QString protocol = QString("<?xml version='1.0' encoding='UTF-8'?>"
    //                               "<entry xmlns=\"http://www.w3.org/2005/Atom\">"
    //                               "<id>%1</id>"
    //                               "<title>%2</title>"
    //                               "</entry>").arg(fileURL.toString()).arg("screenshot.jpg");

    //    postProtocol = protocol.toLatin1();

   // CommonTools::setHeader(request);

    //    request.setRawHeader("Content-Length", QString::number(63102/*postProtocol.size()*/).toLatin1());
    //    request.setRawHeader("Content-Type", "application/atom+xml");
//    request.setRawHeader("Content-Type", "application/json");

//    init();

//    QString protocol = QString("\"kind\": \"drive#file\", \"id\": %1,\"selfLink\": %2").arg(getIDFromURL(source)).arg(source);

//    postData = protocol.toLatin1();

//    QString r(COPY_FILE_FIRST_QUERY_PART + getIDFromURL(source) + COPY_FILE_LAST_QUERY_PART);

//    DEBUG << "r" << r;

//    request.setUrl(QUrl("https://docs.google.com/feeds/default/private/full"));
//    request.setUrl(QUrl(r));

//    //postRequest(QUrl(""), QString(""));

//    //reply = networkManager->post(request, postProtocol);
//    reply = networkManager->post(request, postData);
//    connect(networkManager.data(), SIGNAL(finished(QNetworkReply*)), this, SLOT(slotPostFinished(QNetworkReply*)));
//    connectErrorHandlers();

//    DEBUG << "postProtocol" << postData;
}

void OperationsManager::setStartSettings(QUrl url, const QString &fileName, const QString &progressBarDialogInfoText)
{
    Q_UNUSED(url);
    Q_UNUSED(fileName);
    Q_UNUSED(progressBarDialogInfoText);
}

void OperationsManager::createFolder(const QString &folderUrl, const QString &name)
{
    QString url(folderUrl + CONTENTS);
    DEBUG << "name:"<< name << "URL" << url;

    init();

    QString data = QString("{\"title\": \"%1\",\"parents\": [{\"id\":\"%2\"}],\"mimeType\": \"application/vnd.google-apps.folder\"}").arg(name).arg(getIDFromURL(folderUrl));

    postData = data.toLatin1();

    CommonTools::setHeader(request);

    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Length", QByteArray::number(postData.size()));

    request.setUrl(QUrl("https://www.googleapis.com/drive/v2/files"));

    reply = networkManager->post(request, postData);

    connect(networkManager.data(), SIGNAL(finished(QNetworkReply*)), this, SLOT(slotPostFinished(QNetworkReply*)));
    connectErrorHandlers();
}

QUrl OperationsManager::getDeleteFileQuery(const QString &url)
{
    return QUrl(QString(DELETE_FILE += getIDFromURL(url)));
}

QUrl OperationsManager::getCopyFileQuery(const QString &url)
{
    return QUrl(QString(COPY_FILE += getIDFromURL(url)));
}

QString OperationsManager::getIDFromURL(const QString &url)
{
    int backParamNum = 1;

    if(url.contains(QRegExp("max-results")))
    {
        backParamNum = 2;
    }

    QStringList queryStrs(url.split("/"));
    QString lastParam(queryStrs[queryStrs.count()  - backParamNum]);

    queryStrs = lastParam.split("%3A");

    return queryStrs[queryStrs.count()  - 1];
}

void OperationsManager::slotReplyFinished(QNetworkReply *reply)
{
    Q_UNUSED(reply);
    emit signalDelFinished();
}

void OperationsManager::slotPostFinished(QNetworkReply* reply)
{
    NetworkManager::slotPostFinished(reply);
    SDriveEngine::inst()->getFilesMngr()->get(SDriveEngine::inst()->getFilesMngr()->getUpLevelFolderLink());
}


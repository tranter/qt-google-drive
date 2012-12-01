#include "network/operationsmanager.h"
#include "share/commontools.h"
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

    reply = networkManager->deleteResource(request);

    connect(networkManager.data(), SIGNAL(finished(QNetworkReply*)),this, SLOT(slotReplyFinished(QNetworkReply*)));
    connectErrorHandlers();
}

void OperationsManager::setCopySettings(void)
{
    QUrl fileURL(getCopyFileQuery(getRequest().url().toString()));
    QFileInfo fileInfo(file);

    DEBUG << "fileURL" << fileURL << "fileName" << fileInfo.fileName();

    QString protocol = QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                                   "<entry xmlns=\"http://www.w3.org/2005/Atom\">"
                                   "<id>%1</id>"
                                   "<title>%2</title>"
                                   "</entry>").arg(fileURL.toString()).arg(fileInfo.fileName());

    postProtocol = protocol.toLatin1();

    DEBUG << "postProtocol" << postProtocol;

    CommonTools::setHeader(request);

    request.setRawHeader("Content-Length", QString::number(postProtocol.size()).toLatin1());
    request.setRawHeader("Content-Type", "application/atom+xml");
}

void OperationsManager::copyWebFile(const QString &fileName, const QString &source, const QString &dest)
{
    DEBUG << "source" << source << "dest" << dest;
    postRequest(getCopyFileQuery(source), fileName);
}

void OperationsManager::setStartSettings(QUrl url, const QString &fileName, const QString &progressBarDialogInfoText)
{
    Q_UNUSED(progressBarDialogInfoText);

    NetworkManager::setStartSettings(url, fileName, QString("Copying file: "));
    setCopySettings();
}

void OperationsManager::createFolder(const QString &folderUrl, const QString &name)
{
    QString url(folderUrl + CONTENTS);
    DEBUG << "name:"<< name << "URL" << url;
}

QUrl OperationsManager::getDeleteFileQuery(const QString &url)
{
    return QUrl(QString(DELETE_FILE += getFileIDFromURL(url)));
}

QUrl OperationsManager::getCopyFileQuery(const QString &url)
{
    return QUrl(QString(COPY_FILE += getFileIDFromURL(url)));
}

QString OperationsManager::getFileIDFromURL(const QString &url)
{
    QStringList queryStrs(url.split("/"));
    QString lastParam(queryStrs[queryStrs.count()  - 1]);

    queryStrs = lastParam.split("%3A");

    DEBUG << "queryStrs[queryStrs.count()  - 1]" << queryStrs[queryStrs.count()  - 1];

    return queryStrs[queryStrs.count()  - 1];
}

void OperationsManager::slotReplyFinished(QNetworkReply *reply)
{
    Q_UNUSED(reply);
    emit signalDelFinished();
}


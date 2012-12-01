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

void OperationsManager::copyWebFile(const QString &source, const QString &dest)
{
    DEBUG << "source" << source << "dest" << dest;
}

void OperationsManager::createFolder(const QString &folderUrl, const QString &name)
{
    QString url(folderUrl + CONTENTS);
    DEBUG << "name:"<< name << "URL" << url;
}

QUrl OperationsManager::getDeleteFileQuery(const QString &url)
{
    QString query(DELETE_FILE);
    QStringList queryStrs(url.split("/"));
    QString lastParam(queryStrs[queryStrs.count()  - 1]);

    queryStrs = lastParam.split("%3A");
    query += queryStrs[queryStrs.count()  - 1];

    return QUrl(query);
}

void OperationsManager::slotReplyFinished(QNetworkReply *reply)
{
    Q_UNUSED(reply);
    emit signalDelFinished();
}


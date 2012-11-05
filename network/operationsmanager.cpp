#include "network/operationsmanager.h"
#include "share/commontools.h"
#include "share/debug.h"
#include <QStringList>

OperationsManager::OperationsManager(QObject *parent):
    NetworkManager(parent)
{
}

void OperationsManager::del(const QString& url)
{
    DEBUG << "URL" << url;

    CommonTools::setHeader(request);
    request.setRawHeader("If-Match", "*");

    init();
    request.setUrl(getFileQuery(url));

    reply = networkManager->deleteResource(request);

    connect(networkManager.data(), SIGNAL(finished(QNetworkReply*)),this, SLOT(slotReplyFinished(QNetworkReply*)));
    connectErrorHandlers();
}

void OperationsManager::createFolder(const QString& folderUrl, const QString& name)
{
    QString url(folderUrl + CONTENTS);
    DEBUG << "name:"<< name << "URL" << url;
}

QUrl OperationsManager::getFileQuery(const QString& url)
{
    QString query(DELETE_FILE);
    QStringList queryStrs(url.split("/"));
    QString lastParam(queryStrs[queryStrs.count()  - 1]);

    queryStrs = lastParam.split("%3A");
    query += queryStrs[queryStrs.count()  - 1];

    return QUrl(query);
}

void OperationsManager::slotReplyFinished(QNetworkReply* reply)
{
    Q_UNUSED(reply);
    emit signalDelFinished();
}


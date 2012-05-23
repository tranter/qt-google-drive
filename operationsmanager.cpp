#include "operationsmanager.h"
#include <QDebug>
#include <QStringList>

OperationsManager::OperationsManager():
    networkManager(NULL)
{
}

void OperationsManager::deleteFile(const QString& url)
{
    qDebug() << "OperationsManager::deleteFile" << getFileQuery(url);
}

void OperationsManager::setNetworkManager(const QNetworkAccessManager* manager)
{
    qDebug() << "OperationsManager::setNetworkManager: manager" << manager;
    networkManager = manager;
}

QString OperationsManager::getFileQuery(const QString& url)
{
    QString query(DELETE_FILE);
    QStringList queryStrs(url.split("/"));
    QString lastParam(queryStrs[queryStrs.count()  - 1]);

    queryStrs = lastParam.split("%3A");
    query += queryStrs[queryStrs.count()  - 1];

    return query;
}


#ifndef OPERATIONSMANAGER_H
#define OPERATIONSMANAGER_H

#include <QNetworkAccessManager>
#include "Def.h"

class OperationsManager
{
public:
    OperationsManager();

public:
    void deleteFile(const QString& url);
    void setNetworkManager(const QNetworkAccessManager *manager);

private:
    QString getFileQuery(const QString& url);

private:
    const QNetworkAccessManager* networkManager;

};

#endif // OPERATIONSMANAGER_H

#ifndef OPERATIONSMANAGER_H
#define OPERATIONSMANAGER_H

#include "network/networkmanager.h"
#include "share/defs.h"

class OperationsManager : public NetworkManager
{
     Q_OBJECT
public:
    OperationsManager(QObject *parent = 0);

public:
    void del(const QString& url);
    void createFolder(const QString& folderUrl, const QString& name);

public slots:
    virtual void slotReplyFinished(QNetworkReply* reply);

signals:
    void signalDelFinished(void);

private:
    QUrl getFileQuery(const QString& url);
};

#endif // OPERATIONSMANAGER_H

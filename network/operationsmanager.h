#ifndef OPERATIONSMANAGER_H
#define OPERATIONSMANAGER_H

#include "network/networkmanager.h"
#include "gui/iteminfo.h"
#include "share/defs.h"

class OperationsManager : public NetworkManager
{
     Q_OBJECT
public:
    OperationsManager(QObject *parent = 0);

public:
    void deleteFile(const QString &url);
    void copyWebFile(const ItemInfo::Data &source, const QString &destFolder);
    void createFolder(const QString &folderUrl, const QString &name);

signals:
    void signalDelFinished(void);

protected slots:
    virtual void slotReplyFinished(QNetworkReply *reply);
    virtual void slotPostFinished(QNetworkReply* reply);

protected:
    virtual void setStartSettings(QUrl url, const QString &fileName, const QString &progressBarDialogInfoText);

private:
    QString getIDFromURL(const QString &url);
    QUrl getDeleteFileQuery(const QString &url);
    QUrl getCopyFileQuery(const QString &url);
};

#endif // OPERATIONSMANAGER_H

#ifndef OPERATIONSMANAGER_H
#define OPERATIONSMANAGER_H

#include "network/networkmanager.h"
#include "gui/iteminfo.h"
#include "share/defs.h"

class OperationsManager : public NetworkManager
{
    Q_OBJECT
public:
    enum EOperations
    {
        ENone,
        ECopy,
        ECreateFolder,
        EDelete,
        ERename,
        EShare
    };

    OperationsManager(QObject *parent = 0);

public:
    void deleteFile(const QString &sourceUrl);
    void copyWebFile(const Items::Data &source, const QString &destFolder);
    void moveWebFile(const Items::Data &source, const QString &destFolder);
    void renameWebFile(const Items::Data &source, const QString &newName);
    void shareWebFile(const Items::Data &source);
    void createFolder(const QString &folderUrl, const QString &name);

protected slots:
    virtual void slotReplyFinished(QNetworkReply*);
    virtual void slotPostFinished(QNetworkReply* reply);

private slots:
    void slotPutFinished(void);

private:
    QString getIDFromURL(const QString &url);
    QUrl getDeleteFileQuery(const QString &url);
    QUrl getCopyFileQuery(const QString &url);
    void updatePanelContent(bool opposite);

private:
    EOperations currentOperation;
    QString fileURLToDeleteForMoveOperation;
    bool isMove;
};

#endif // OPERATIONSMANAGER_H

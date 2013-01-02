#ifndef OPERATIONSMANAGER_H
#define OPERATIONSMANAGER_H

#include "network/networkmanager.h"
#include "gui/iteminfo.h"
#include "settings/accountinfo.h"
#include "share/singleton.h"
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
    void copyWebFile(const Items::Data &source, const QString &destFolderUrl);
    void moveWebFile(const Items::Data &source, const QString &destFolder);
    void renameWebFile(const Items::Data &source, const QString &newName);
    void shareWebFile(const Items::Data &source);
    void createFolder(const QString &folderUrl, const QString &name);
    void setAccountInfo(const QString &accessToken, const QString &refreshToken = QString());

signals:
    void signalAccountInfoReadyToUse(void);

protected slots:
    virtual void slotReplyFinished(QNetworkReply*);
    virtual void slotPostFinished(QNetworkReply* reply);

private slots:
    void slotPutFinished(void);
    void slotAccountInfo(AccountInfo::Data &data);

private:
    QUrl getDeleteFileQuery(const QString &url);
    void updatePanelContent(bool opposite);

private:
    EOperations currentOperation;
    QString fileUrlToDeleteForMoveOperation;
    bool isMove;
    AccountInfo *accountInfo;
};

typedef TSingleton<OperationsManager> SOperationsManager;

#endif // OPERATIONSMANAGER_H

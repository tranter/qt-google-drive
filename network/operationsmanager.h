#ifndef OPERATIONSMANAGER_H
#define OPERATIONSMANAGER_H

#include "network/networkmanager.h"
#include "gui/items.h"
#include "settings/accountinfo.h"
#include "share/singleton.h"
#include "share/defs.h"
#include "queries.h"
#include "gui/forms/createfolderdialog.h"

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

public slots:
    void slotDelete(void);
    void slotNewFolder(void);
    void slotCopyWebFile(void);
    void slotMoveWebFile(void);
    void slotRenameWebFile(void);
    void slotItemEditDone(void);
    void slotShareWebFile(void);
    void slotAcceptCreateFolder(const QString &name);
    void slotRejectCreateFolder(void);
    void slotFinishedCreateFolder(int result);

public:
    void deleteFile(const Items::Data &source);
    void copyWebFile(const Items::Data &source, const QString &destFolderUrl);
    void moveWebFile(const Items::Data &source, const QString &destFolderUrl);
    void renameWebFile(const Items::Data &source, const QString &newName);
    void shareWebFile(const Items::Data &source);
    void createFolder(const QString &name, const QString &folderUrl);
    void setAccountInfo(const QString &accessToken, const QString &refreshToken = QString());
    bool operationPossible(void);

signals:
    void signalAccountInfoReadyToUse(void);

protected slots:
    virtual void slotReplyFinished(QNetworkReply*);
    virtual void slotPostFinished(QNetworkReply* reply);
    void slotPutFinished(void);

private slots:  
    void slotAccountInfoReceived(AccountInfo::Data &postData);

private:
    void updatePanelContent(bool opposite);

private:
    EOperations currentOperation;
    Items::Data fileUrlToDeleteForMoveOperation;
    bool isMove;
    AccountInfo *accountInfo;
    Queries queries;
    CreateFolderDialog* createFolderDialog;
    QString editingItemText;
};

typedef TSingleton<OperationsManager> SOperationsManager;

#endif // OPERATIONSMANAGER_H

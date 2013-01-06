#ifndef OPERATIONSMANAGER_H
#define OPERATIONSMANAGER_H

#include "network/networkmanager.h"
#include "gui/items.h"
#include "settings/accountinfo.h"
#include "share/singleton.h"
#include "share/defs.h"
#include "network/queries.h"
#include "gui/forms/createfolderdialog.h"

#include "network/operations/copy.h"
#include "network/operations/delete.h"
#include "network/operations/move.h"
#include "network/operations/create.h"

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
    void renameWebFile(const Items::Data &source, const QString &newName);
    void shareWebFile(const Items::Data &source);
    void setAccountInfo(const QString &accessToken, const QString &refreshToken = QString());
    bool operationPossible(void);
    void updatePanelContent(bool opposite);

signals:
    void signalAccountInfoReadyToUse(void);

protected slots:
    void slotReplyFinished(QNetworkReply*);
    void slotPutFinished(void);

private slots:  
    void slotAccountInfoReceived(AccountInfo::Data &postData);

private:
    EOperations currentOperation;
    Items::Data fileUrlToDeleteForMoveOperation;
    AccountInfo *accountInfo;
    Queries queries;
    CreateFolderDialog* createFolderDialog;
    QString editingItemText;
    Copy copy;
    Delete del;
    Move move;
    Create create;
};

typedef TSingleton<OperationsManager> SOperationsManager;

#endif // OPERATIONSMANAGER_H

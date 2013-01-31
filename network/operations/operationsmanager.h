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
#include "network/operations/rename.h"
#include "network/operations/share.h"
#include "network/operations/children.h"

class OperationsManager : public NetworkManager
{
    Q_OBJECT
public:
    OperationsManager(QObject *parent = 0);

public:
    void setAccountInfo(const QString &accessToken, const QString &refreshToken = QString());
    bool operationPossible(void);

signals:
    void signalAccountInfoReadyToUse(void);

public slots:
    void slotDelete(void);
    void slotNewFolder(void);
    void slotCopy(void);
    void slotMove(void);
    void slotRename(void);
    void slotItemEditDone(void);
    void slotShare(void);
    void slotAcceptCreateFolder(const QString &name);
    void slotRejectCreateFolder(void);
    void slotFinishedCreateFolder(int result);

private slots:
    void slotAccountInfoReceived(AccountInfo::Data &postData);
    void slotItemOperationCompleted(Items::Data &itemData);

private:
    Items::Data fileUrlToDeleteForMoveOperation;
    AccountInfo *accountInfo;
    Queries queries;
    CreateFolderDialog* createFolderDialog;
    QString editingItemText;
    Copy copy;
    Delete del;
    Move move;
    Create create;
    Rename rename;
    Share share;
    Children children;
};

typedef TSingleton<OperationsManager> SOperationsManager;

#endif // OPERATIONSMANAGER_H

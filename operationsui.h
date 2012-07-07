#ifndef OPERATIONSUI_H
#define OPERATIONSUI_H

#include "driveengine.h"
#include "createfolderdialog.h"

class OperationsUI: public QObject
{
    Q_OBJECT
public:
    explicit OperationsUI(QObject *parent = 0);

private slots:
    void slotCreateFolder();
    void slotAcceptCreateFolder(const QString &name);
    void slotRejectCreateFolder();
    void slotFinishedCreateFolder(int result);

    void slotDel(QObject* object);
    void slotTriggeredDel();
    void slotDelFinished();

private:
    void createFolder(const QString& name);
    void delItemInTree(TreeItemInfo item);

private:
    CreateFolderDialog* createFolderDialog;
};

#endif // OPERATIONSUI_H

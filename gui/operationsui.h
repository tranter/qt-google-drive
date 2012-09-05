#ifndef OPERATIONSUI_H
#define OPERATIONSUI_H

#include "driveengine.h"
#include "gui/forms/createfolderdialog.h"

class OperationsUI: public QObject
{
    Q_OBJECT
public:
    explicit OperationsUI(QObject *parent = 0);

public:
   void del(QObject* object);

private slots:
    void slotNewFolder();
    void slotAcceptCreateFolder(const QString &name);
    void slotRejectCreateFolder();
    void slotFinishedCreateFolder(int result);

    //void slotDel(QObject* object);

    void slotTriggeredDel();
    void slotDelFinished();

private:
    void createFolder(const QString& name);
    void delItemInTree(ItemInfo item);

private:
    CreateFolderDialog* createFolderDialog;
};

#endif // OPERATIONSUI_H

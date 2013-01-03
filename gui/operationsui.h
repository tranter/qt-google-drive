#ifndef OPERATIONSUI_H
#define OPERATIONSUI_H

#include "core/driveengine.h"
#include "gui/forms/createfolderdialog.h"

class OperationsUI: public QObject
{
    Q_OBJECT
public:
    explicit OperationsUI(QObject *parent = 0);

//public:
//   void del(void);

public slots:
    void slotNewFolder(void);
    void slotCopyWebFile(void);
    void slotMoveWebFile(void);
    void slotRenameWebFile(void);
    void slotShareWebFile(void);
    void slotAcceptCreateFolder(const QString &name);
    void slotRejectCreateFolder(void);
    void slotFinishedCreateFolder(int result);
    //void slotTriggeredDel(void);
    void slotItemEditDone(void);

private:
    void createFolder(const QString &name);
    bool operationPossible(void);

private:
    CreateFolderDialog* createFolderDialog;
    QString editingItemText;
};

#endif // OPERATIONSUI_H

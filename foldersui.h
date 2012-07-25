#ifndef FOLDERSUI_H
#define FOLDERSUI_H

#include "driveengine.h"
//#include "enums.h"
#include "iteminfo.h"

class FoldersUI: public QObject
{
    Q_OBJECT
public:
   explicit FoldersUI(QObject *parent = 0);

   friend class DriveEngine;
   friend class FilesTransferUI;
   friend class FilesUI;
   friend class OperationsUI;

private slots:
    void slotFoldersViewClicked(const QModelIndex& index);

private:
    void createAFolders(void);
    bool getFolderContent(QString &ID);
    int getCurrFolderItemId(void) const;
    bool isFolder(void);
    ItemInfo::Data item(void);
    void showFolders(const QString &url);

private:
    QModelIndex currAFolderId;
};

#endif // FOLDERSUI_H

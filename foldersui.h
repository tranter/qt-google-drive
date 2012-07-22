#ifndef FOLDERSUI_H
#define FOLDERSUI_H

#include "driveengine.h"

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
    bool getFolderContent(QString &folderID);
    int getCurrFolderItemId(void) const;
    void createAFolders(void);
    void showFolders(const QString &url);

private:
    QModelIndex currAFolderId;
    //int currentFolderIndex;
};

#endif // FOLDERSUI_H

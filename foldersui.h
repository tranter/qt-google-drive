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
    bool folderInFilesView(QString &resourceID);
    int getCurrentFolderItemIndex(void) const;
    void showAdditionalFolders(void);
    void showFolders(void);

private:
    QModelIndex currentAdditionalFolderIndex;
    int currentFolderIndex;
};

#endif // FOLDERSUI_H

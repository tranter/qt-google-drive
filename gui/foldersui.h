#ifndef FOLDERSUI_H
#define FOLDERSUI_H

#include "core/driveengine.h"
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

private:
    //void createAFolders(void);
    QString getFolderID(void);
    bool isFolder(void);

private:
    QModelIndex currAFolderId;
};

#endif // FOLDERSUI_H

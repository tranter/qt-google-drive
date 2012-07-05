#ifndef FILESUI_H
#define FILESUI_H

#include <QObject>
#include "driveengine.h"

class FilesManager;

class FilesUI: public QObject
{
    Q_OBJECT
public:
    explicit FilesUI(QObject *parent = 0);

    friend class DriveEngine;
    friend class FilesTransferUI;

private slots:
    void slotAdditionalShowFiles(const QModelIndex& index);
    void slotFilesViewClicked(const QModelIndex&);
    void slotUpdateFileList();

private:
    int getCurrentFileItemIndex(FilesManager* manager) const;
    void showFiles(void);
    void showFilesFromFolderInFilesView(void);
};

#endif // FILESUI_H

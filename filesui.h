#ifndef FILESUI_H
#define FILESUI_H

#include "driveengine.h"

class FilesManager;

class FilesUI: public QObject
{
    Q_OBJECT
public:
    explicit FilesUI(QObject *parent = 0);

    friend class FilesTransferUI;
    friend class FoldersUI;
    friend class OperationsUI;

private slots:
    void slotAdditionalShowFiles(const QModelIndex& index);
    void slotFilesSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
    void slotFilesViewClicked(const QModelIndex&);
    void slotUpdateFileList();

private:
    int getCurrentFileItemIndex(FilesManager* manager) const;
    void showFiles(void);
    void showFilesFromFolderInFilesView(void);
};

#endif // FILESUI_H

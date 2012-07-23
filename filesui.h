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
    void slotAShowFiles(const QModelIndex &index);
    void slotLeftViewClicked(const QModelIndex& Id);
    void slotRightViewClicked(const QModelIndex& Id);
    void slotLeftSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
    void slotRightSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
    void slotUpdateFileList();

private:
    int getCurrFileItemId(FilesManager* manager) const;
    void showFiles(void);
    void showFilesFromFolder(void);
    void showFilesOnPanel(const QModelIndex& Id);
};

#endif // FILESUI_H

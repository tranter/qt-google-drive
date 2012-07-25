#ifndef FILESUI_H
#define FILESUI_H

#include "driveengine.h"
#include "enums.h"

class FilesManager;

class FilesUI: public QObject
{
    Q_OBJECT
public: 
    enum EPath
    {
        EForward = 0,
        EBackward
    };

    explicit FilesUI(QObject *parent = 0);

    friend class FilesTransferUI;
    friend class FoldersUI;
    friend class OperationsUI;
    friend class DriveEngine;

private slots:
    void slotAShowFiles(const QModelIndex &index);
    void slotLeftViewClicked(const QModelIndex& Id);
    void slotRightViewClicked(const QModelIndex& Id);
    void slotLeftSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
    void slotRightSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
    void slotUpdateFileList();

private:
    int getCurrFileItemId(FilesManager* manager) const;
    QString getDisc(EPanels panel) const;
    QLabel* getPanelLabel(EPanels panel) const;
    void setDisc(EPanels panel);
    void setPath(EPath path, EPanels panel);
    void showFiles(void);
    void showFilesFromFolder(void);
    void showFilesOnPanel(const QModelIndex& Id, EPanels panel);
};

#endif // FILESUI_H

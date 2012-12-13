#ifndef FILESUI_H
#define FILESUI_H

#include "core/driveengine.h"
#include "share/enums.h"

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

public:
    QLabel* getPanelLabel(EPanels panel) const;

private slots:
    //void slotAShowFiles(const QModelIndex &index);
    void slotLeftViewClicked(const QModelIndex &Id);
    void slotRightViewClicked(const QModelIndex &Id);
    void slotLeftPanelItemDoubleClicked(QTreeWidgetItem *item, int column);
    void slotRightPanelItemDoubleClicked(QTreeWidgetItem *item, int column);
    void slotLeftSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
    void slotRightSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
    void slotUpdateFileList();

private:
    QString getDisc(EPanels panel) const;
    //void setDisplayingDisc(EPanels panel);
    void setPanelDisplayingPath(const QString &name, EPath path, EPanels panel);
    void showFilesOnPanel(const QString &name, EPanels panel);
    void setCurrentPanelState(EPanels panel, const QString &URL);
    void performShowFiles(const QString &query, const QString &name, EPath path, EPanels panel);
};

#endif // FILESUI_H

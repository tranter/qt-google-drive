#ifndef FILEPANEL_H
#define FILEPANEL_H

#include <QWidget>
#include <QTreeWidget>
#include <QLabel>
#include "gui/controls/toolbar.h"
#include "gui/controls/combobox.h"
#include "gui/controls/toolbutton.h"
#include "gui/tools/filesystemmodel.h"
#include "network/webcontentmanager.h"

namespace Ui {
class FilePanel;
}

class FilePanel : public QWidget
{
    Q_OBJECT
public:
      enum EPath { EForward, EBackward };

    explicit FilePanel(int pn, QWidget *parent = 0);
    ~FilePanel();

private slots:
    void slotAccountsComboBoxActivated(const QString &text);
    void slotComputerDrivesButtonClicked();  
    void slotItemPressed(QTreeWidgetItem *item, int column);
    void slotPanelItemDoubleClicked(QTreeWidgetItem *item, int column);
    void slotCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void slotItemEntered(QTreeWidgetItem *item, int column);
    void slotUpdateFileList(void);


//    void slotComputerDrivesViewClicked(const QModelIndex& index);
//    void slotComputerDrivesViewDoubleClicked(const QModelIndex& index);

public:
    QTreeWidget *getFileView(void) const;
    QLabel *getPathLabel(void) const;
    void fillComboBox(QMap<QString, QString> accountsMap, const QString &currentAccount);
    void update();
    WebContentManager* getWebContentMngr() const;
    QList<int> getMarkedItemIds(QTreeWidget *treeWidget) const;
    void markItem(QTreeWidgetItem *item, bool noSwitch = false);
    void markItems(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    bool hasItemParentSign(QTreeWidgetItem *item) const;

private:
    void init(void);
    int getDiscLength() const;
    void setPanelDisplayingPath(const QString &name, EPath path);
    void showFilesOnPanel(QTreeWidgetItem *item);
    void setCurrentPanelState(const QString &url);
    void performShowFiles(const QString &query, const QString &name, EPath path);
    bool isFolder(void);
    int itemIndex(QTreeWidgetItem *item);
    void itemPressed(QTreeWidgetItem *item);
    void itemDoubleClicked(QTreeWidgetItem *item);
    void currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void markItemWithMouseTracking(QTreeWidgetItem *item);
    void markItemWithMousePress(QTreeWidgetItem *item);

private:
    Ui::FilePanel *ui;

private:
    QScopedPointer<WebContentManager> webContentMngr;
    ToolBar *accountsToolBar;
    ComboBox *accountsComboBox;
    QToolButton *computerDrivesButton;
    //QScopedPointer<FileSystemModel> fileSystemModel;
    //QScopedPointer<QTreeView> computerDrivesView;
    int panelNum;
    bool mouseTracking;
};

#endif // FILEPANEL_H

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
#include "network/computercontentmanager.h"
#include "share/enums.h"

namespace Ui {
class FilePanel;
}

class FilePanel : public QWidget
{
    Q_OBJECT
public:
    friend class ContentManager;

    explicit FilePanel(int pn, QWidget *parent = 0);
    ~FilePanel();

signals:
    void computerPanelActivated(int panelNum);

public slots:
    void slotUpdate();

private slots:
    void slotAccountsComboBoxActivated(const QString &text);
    void slotComputerDrivesButtonClicked();
    void slotItemPressed(QTreeWidgetItem *item, int column);
    void slotPanelItemDoubleClicked(QTreeWidgetItem *item, int column);
    void slotCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void slotItemEntered(QTreeWidgetItem *item, int column);
    void slotHeaderSectionClicked(int logicalIndex);

public:
    QTreeWidget *getFileView() const;
    QLabel *getPathLabel() const;
    ContentManager *getContentMngr() const;
    QToolButton *getComputerDrivesButton() const;
    void update();
    QList<int> getMarkedItemIds(QTreeWidget *treeWidget) const;
    void markItem(QTreeWidgetItem *item, bool noSwitch = false);
    void computerDrivesButtonClicked();

private:
    void init(void);
    void showFilesOnPanel(QTreeWidgetItem *item);
    int itemIndex(QTreeWidgetItem *item);
    void itemPressed(QTreeWidgetItem *item);
    void itemDoubleClicked(QTreeWidgetItem *item);
    void currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void markItemWithMouseTracking(QTreeWidgetItem *item);
    void markItemWithMousePress(QTreeWidgetItem *item);
    void markItems(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void setConnections();
    void createAndSetAccountsToolBarWithWidgets();
    void setupUiWidgets();

private:
    Ui::FilePanel *ui;

private:
    QScopedPointer<ContentManager> contentMngrs[EPanelStateCount];
    QScopedPointer<ComputerContentManager> computerContentManager;
    ToolBar *accountsToolBar;
    ComboBox *drivesComboBox;
    QToolButton *computerDrivesButton;
    int panelNum;
    bool mouseTracking;
    EPanelState panelState;
};

#endif // FILEPANEL_H

#include "filepanel.h"
#include "ui_filepanel.h"
#include "share/defs.h"
#include "share/debug.h"
#include "gui/controls/spacer.h"
#include "settings/settingsmanager.h"
#include "core/driveengine.h"
//#include "gui/tools/fileiconprovider.h"
#include <QDir>
#include <QApplication>
#include <QFont>

FilePanel::FilePanel(int pn, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilePanel),
    panelNum(pn)
{    
    init();
}

FilePanel::~FilePanel()
{
    delete ui;
}

void FilePanel::init(void)
{
    ui->setupUi(this);

    panelState =  static_cast<EPanelState> (SettingsManager().panelState(panelNum));

    setupUiWidgets();

    createAndSetAccountsToolBarWithWidgets();

    contentMngrs[EWeb].reset(new WebContentManager(this));
    contentMngrs[EComputer].reset(new ComputerContentManager(this));

    setConnections();
}

void FilePanel::setConnections()
{
    connect(drivesComboBox, SIGNAL(activated(const QString&)), SLOT(slotAccountsComboBoxActivated(const QString&)));
    connect(computerDrivesButton, SIGNAL(clicked()), SLOT(slotComputerDrivesButtonClicked()));
    connect(ui->fileView, SIGNAL(itemPressed(QTreeWidgetItem*, int)), SLOT(slotItemPressed(QTreeWidgetItem*, int)));
    connect(ui->fileView, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), SLOT(slotPanelItemDoubleClicked(QTreeWidgetItem*, int)));
    connect(ui->fileView, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), SLOT(slotCurrentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
    connect(ui->fileView, SIGNAL(itemEntered(QTreeWidgetItem*, int)), SLOT(slotItemEntered(QTreeWidgetItem*, int)));
    connect(ui->fileView->header(),SIGNAL(sectionClicked(int)), SLOT(slotHeaderSectionClicked(int)));
}

void FilePanel::createAndSetAccountsToolBarWithWidgets()
{
    accountsToolBar = new ToolBar;
    ui->verticalLayout->insertWidget(0, accountsToolBar);

    drivesComboBox = new ComboBox;
    computerDrivesButton = new ToolButton;

    accountsToolBar->addWidget(drivesComboBox);
    accountsToolBar->addWidget(new Spacer(this));
    accountsToolBar->addWidget(computerDrivesButton);

    switch(panelState)
    {
    case EWeb: computerDrivesButton->setChecked(false);
        break;
    case EComputer: computerDrivesButton->setChecked(true);
        break;
    case EPanelStateCount :break;
    }
}

void FilePanel::setupUiWidgets()
{
    ui->pathLabel->setFixedHeight(18);

    ui->fileView->header()->setClickable(true);
    ui->fileView->header()->resizeSection(0, 250);
    ui->fileView->header()->resizeSection(1, 60);
    ui->fileView->header()->resizeSection(2, 150);
}

ContentManager* FilePanel::getContentMngr() const
{
    return contentMngrs[panelState].data();
}

void FilePanel::slotAccountsComboBoxActivated(const QString &text)
{
    contentMngrs[panelState]->accountsComboBoxItemActivated(text);
}

void FilePanel::computerDrivesButtonClicked()
{
    if(computerDrivesButton->isChecked()) panelState = EComputer;
    else panelState = EWeb;

    if(panelState == EComputer) emit computerPanelActivated(panelNum);

    SettingsManager().setPanelState(panelNum, panelState);
    update();
}

void FilePanel::slotComputerDrivesButtonClicked()
{
    computerDrivesButtonClicked();
}

QTreeWidget *FilePanel::getFileView(void) const
{
    return ui->fileView;
}

QLabel *FilePanel::getPathLabel(void) const
{
    return ui->pathLabel;
}

QToolButton *FilePanel::getComputerDrivesButton(void) const
{
    return computerDrivesButton;
}

void FilePanel::slotUpdate()
{
    update();
//    #include <QDesktopServices>
//    QDesktopServices::DataLocation
}

void FilePanel::update()
{
    contentMngrs[panelState]->update();
}

void FilePanel::slotItemPressed(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    itemPressed(item);
    //DEBUG << item;
}

void FilePanel::slotPanelItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    itemDoubleClicked(item);
}

void FilePanel::slotCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    currentItemChanged(current, previous);
    //    DEBUG << current;
    //    DEBUG << previous;
}

void FilePanel::slotItemEntered(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    markItemWithMouseTracking(item);
    //DEBUG << item;
}

void FilePanel::slotHeaderSectionClicked(int logicalIndex)
{
    contentMngrs[panelState]->headerSectionClicked(logicalIndex);
}

int FilePanel::itemIndex(QTreeWidgetItem *item)
{
    int index = -1;

    for(int i = 0; i < item->treeWidget()->topLevelItemCount(); ++i)
    {
        if(item == item->treeWidget()->topLevelItem(i))
        {
            index = i;
            break;
        }
    }

    return index;
}

void FilePanel::markItem(QTreeWidgetItem *item, bool noSwitch)
{
    if(contentMngrs[panelState]->hasItemParentSign(item)) return;

    QBrush brush;

    if(noSwitch) brush.setColor(Qt::red);
    else brush.setColor(Qt::black);

    if(item->foreground(0).color() == Qt::black) brush.setColor(Qt::red);

    for(int i = 0; i < item->treeWidget()->columnCount(); ++i)
    {
        item->setForeground(i, brush);
    }

   //DEBUG << item->data(0, Qt::DisplayRole).toString();
}

void FilePanel::markItems(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    int currentIndex = itemIndex(current);
    int previousIndex = itemIndex(previous);
    int beginIndex, endIndex;

    if(currentIndex < previousIndex)
    {
        beginIndex = currentIndex;
        endIndex = previousIndex;
    }
    else
    {
        beginIndex = previousIndex;
        endIndex = currentIndex;
    }

    for(int i = beginIndex; i <= endIndex; ++i)
    {
        markItem(current->treeWidget()->topLevelItem(i), true);
    }
}

void FilePanel::itemPressed(QTreeWidgetItem *item)
{
    SettingsManager().setCurrentPanel(panelNum);
    mouseTracking = false;
    markItemWithMousePress(item);
}

void FilePanel::itemDoubleClicked(QTreeWidgetItem *item)
{
    SettingsManager().setCurrentPanel(panelNum);
    showFilesOnPanel(item);
}

void FilePanel::currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    SettingsManager().setCurrentPanel(panelNum);

    if(previous)
    {
        if(QApplication::mouseButtons() == Qt::LeftButton && QApplication::keyboardModifiers() == Qt::ShiftModifier) markItems(current, previous);
    }
}

void FilePanel::markItemWithMouseTracking(QTreeWidgetItem *item)
{
    if(mouseTracking)
    {
        if(QApplication::mouseButtons() == Qt::RightButton) markItem(item);
    }

    mouseTracking = true;
}

void FilePanel::markItemWithMousePress(QTreeWidgetItem *item)
{
    if(QApplication::mouseButtons() == Qt::RightButton) markItem(item);
    if(QApplication::mouseButtons() == Qt::LeftButton && QApplication::keyboardModifiers() == Qt::ControlModifier) markItem(item, true);
}

QList<int> FilePanel::getMarkedItemIds(QTreeWidget *treeWidget) const
{
    QList<int> markedItemIds;
    int itemsCount = treeWidget->topLevelItemCount();
    int shiftItemsValue = 0;

    if(itemsCount > 0)
    {
        if(contentMngrs[panelState]->hasItemParentSign(treeWidget->topLevelItem(0))) shiftItemsValue = 1;
    }

    for(int i = 0; i < itemsCount; ++i)
    {
        if(treeWidget->topLevelItem(i)->foreground(0).color() == Qt::red) markedItemIds << (i - shiftItemsValue);
    }

    DEBUG << markedItemIds;

    return markedItemIds;
}

void FilePanel::showFilesOnPanel(QTreeWidgetItem *item)
{
    contentMngrs[panelState]->showFilesOnPanel(item);
}


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

    webContentMngr.reset(new WebContentManager(ui->fileView, panelNum));

    ui->pathLabel->setFixedHeight(18);

    ui->fileView->header()->setClickable(true);
    ui->fileView->header()->resizeSection(0, 250);
    ui->fileView->header()->resizeSection(1, 60);
    ui->fileView->header()->resizeSection(2, 150);

    accountsToolBar = new ToolBar;
    ui->verticalLayout->insertWidget(0, accountsToolBar);

    accountsComboBox = new ComboBox;
    computerDrivesButton = new ToolButton;

    accountsToolBar->addWidget(accountsComboBox);
    accountsToolBar->addWidget(new Spacer(this));
    accountsToolBar->addWidget(computerDrivesButton);


//        QStringList columnNames;
//        columnNames << tr("Name")
//                    <<  tr("Size")
//                    #ifdef Q_OS_MAC
//                     << tr("Kind")
//                    #else
//                     << tr("Type")
//                    #endif
//                     << "Date Modified";

//        fileSystemModel.reset(new FileSystemModel(columnNames));
//        computerDrivesView.reset(new QTreeView);
//        ui->verticalLayout->addWidget(computerDrivesView.data());

//        QFont font(ui->fileView->font());
//        font.setBold(true);

//        computerDrivesView->hide();

//        computerDrivesView->setModel(fileSystemModel.data());
//        computerDrivesView->setFont(font);
//        computerDrivesView->setColumnWidth(0, 220);
//        computerDrivesView->setSortingEnabled(true);
//        computerDrivesView->sortByColumn(0, Qt::AscendingOrder);
//        computerDrivesView->setRootIsDecorated(false);
//        computerDrivesView->setCursor(Qt::PointingHandCursor);
//        computerDrivesView->setIconSize(QSize(32,32));
//        connect(computerDrivesView.data(), SIGNAL(clicked(const QModelIndex&)), this, SLOT(slotComputerDrivesViewClicked(const QModelIndex&)));
//        connect(computerDrivesView.data(), SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(slotComputerDrivesViewDoubleClicked(const QModelIndex&)));

    connect(accountsComboBox, SIGNAL(activated(const QString&)), SLOT(slotAccountsComboBoxActivated(const QString&)));
    connect(computerDrivesButton, SIGNAL(clicked()), SLOT(slotComputerDrivesButtonClicked()));
    connect(ui->fileView, SIGNAL(itemPressed(QTreeWidgetItem*, int)), SLOT(slotItemPressed(QTreeWidgetItem*, int)));
    connect(ui->fileView, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), SLOT(slotPanelItemDoubleClicked(QTreeWidgetItem*, int)));
    connect(ui->fileView, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), SLOT(slotCurrentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
    connect(ui->fileView, SIGNAL(itemEntered(QTreeWidgetItem*, int)), SLOT(slotItemEntered(QTreeWidgetItem*, int)));
}

WebContentManager* FilePanel::getWebContentMngr() const
{
  return webContentMngr.data();
}

void FilePanel::slotAccountsComboBoxActivated(const QString &text)
{
    int beginPos = text.indexOf(ACCOUNT_SEPARATOR_BEGIN) + ACCOUNT_SEPARATOR_BEGIN.length();
    int length = text.lastIndexOf(ACCOUNT_SEPARATOR_END) - beginPos;

    QString accountName(text.mid(beginPos, length));
    SettingsManager settingsManager;

    if(settingsManager.currentAccount(panelNum) != accountName)
    {
        settingsManager.setCurrentAccount(panelNum, accountName);
        update();
    }
}

void FilePanel::slotComputerDrivesButtonClicked()
{
    DEBUG << "panelNum" << panelNum << " state" << computerDrivesButton->isChecked();

    if(computerDrivesButton->isChecked())
    {
        //ui->fileView->hide();
        //computerDrivesView->show();
    }
    else
    {
        //ui->fileView->show();
        //computerDrivesView->hide();
    }
}

//void FilePanel::slotComputerDrivesViewClicked(const QModelIndex &index)
//{
//    DEBUG << fileSystemModel->filePath(index);
//}

//void FilePanel::slotComputerDrivesViewDoubleClicked(const QModelIndex &index)
//{
//    if(fileSystemModel->isDir(index))
//        computerDrivesView->setRootIndex(fileSystemModel->index(fileSystemModel->filePath(index)));
//}

QTreeWidget *FilePanel::getFileView(void) const
{
    return ui->fileView;
}

QLabel *FilePanel::getPathLabel(void) const
{
    return ui->pathLabel;
}

void FilePanel::fillComboBox(QMap<QString, QString> accountsMap, const QString &currentAccount)
{
    QStringList keys(accountsMap.keys());

    accountsComboBox->clear();

    for(int i = 0; i < keys.count(); ++i)
    {
        QString discLetter(keys[i]);

        discLetter = discLetter.rightJustified(2,' ');
        discLetter = discLetter.leftJustified(6, ' ');

        accountsComboBox->addItem(discLetter + ACCOUNT_SEPARATOR_BEGIN + accountsMap[keys[i]] + ACCOUNT_SEPARATOR_END);
        accountsComboBox->setItemIcon(i, QIcon(QApplication::style()->standardIcon(QStyle::SP_DriveFDIcon)));

        if(currentAccount == accountsMap[keys[i]] && accountsComboBox->currentIndex() != i)
        {
            accountsComboBox->setCurrentIndex(i);
        }
    }

    accountsComboBox->setMinimumWidth(80);
}

void FilePanel::update()
{
    SettingsManager settingsManager;
    QString disc;
    QString accountName(settingsManager.currentAccount(panelNum));

    settingsManager.setCurrentPanel(panelNum);

    disc = settingsManager.accountDisc(accountName);
    disc += QString(":");
    disc += QDir::toNativeSeparators("/");

    ui->pathLabel->setText(disc + settingsManager.currentFolderPath(panelNum));
    accountsComboBox->setToolTip("Email: " + accountName + "\nName: " + settingsManager.name(accountName));

    webContentMngr->setPathesURLs(settingsManager.pathesURLs(panelNum));
    //webContentMngr->setPanel(ui->fileView, panelNum);
    webContentMngr->get(settingsManager.currentFolderURL(panelNum));

    fillComboBox(settingsManager.accountsWithLetters(), accountName);
}

void FilePanel::slotItemPressed(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    itemPressed(item);
}

void FilePanel::slotPanelItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    itemDoubleClicked(item);
}

void FilePanel::slotCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    currentItemChanged(current, previous);
}

void FilePanel::slotItemEntered(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    markItemWithMouseTracking(item);
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
    if(hasItemParentSign(item)) return;

    QBrush brush;

    if(noSwitch) brush.setColor(Qt::red);
    else brush.setColor(Qt::black);

    if(item->foreground(0).color() == Qt::black) brush.setColor(Qt::red);

    for(int i = 0; i < item->treeWidget()->columnCount(); ++i)
    {
        item->setForeground(i, brush);
    }
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
        if(hasItemParentSign(treeWidget->topLevelItem(0))) shiftItemsValue = 1;
    }

    for(int i = 0; i < itemsCount; ++i)
    {
        if(treeWidget->topLevelItem(i)->foreground(0).color() == Qt::red) markedItemIds << (i - shiftItemsValue);
    }

    DEBUG << markedItemIds;

    return markedItemIds;
}

bool FilePanel::hasItemParentSign(QTreeWidgetItem *item) const
{
    return item->data(0, Qt::DisplayRole).toString() == PARENT_FOLDER_SIGN;
}

void FilePanel::showFilesOnPanel(QTreeWidgetItem *item)
{
    const QString itemName(item->data(0, Qt::DisplayRole).toString());

    if(hasItemParentSign(item))
    {
        performShowFiles(webContentMngr->back(), itemName, EBackward);
    }
    else
    {
        if(isFolder())
        {
            QString query(GET_FILES_IN_FOLDER);

            query += CommonTools::getIDFromURL(webContentMngr->getCurrentItem().self);
            query += (CONTENTS + MAX_RESULTS);

            performShowFiles(query, itemName, EForward);
        }
    }
}

bool FilePanel::isFolder(void)
{
    return (webContentMngr->getCurrentItem().type == FOLDER_TYPE_STR);
}

void FilePanel::setCurrentPanelState(const QString &url)
{
    SettingsManager settingsManager;

    settingsManager.setCurrentFolderURL(panelNum, url);

    QString fullPath(ui->pathLabel->text());
    int beginPos = fullPath.indexOf(QDir::toNativeSeparators("/")) + 1;
    int length = fullPath.length() - beginPos;

    settingsManager.setCurrentFolderPath(panelNum, fullPath.mid(beginPos, length));
    settingsManager.setPathesURLs(panelNum, webContentMngr->getPathesURLs());
}

void FilePanel::performShowFiles(const QString &query, const QString &name, EPath path)
{
    setPanelDisplayingPath(name, path);
    webContentMngr->get(query);
    setCurrentPanelState(query);
}

void FilePanel::slotUpdateFileList(void)
{
    webContentMngr->get(webContentMngr->getParentFolder());
}

void FilePanel::setPanelDisplayingPath(const QString &name, EPath path)
{
    QString pathDividerSign(QDir::toNativeSeparators("/"));
    int labelTextlength = ui->pathLabel->text().length();
    int discStrLength = getDiscLength();

    switch(path)
    {
    case EForward:
    {
        QString divider((labelTextlength == discStrLength) ? "" : pathDividerSign);
        ui->pathLabel->setText(ui->pathLabel->text() += (divider + name));
    }
        break;
    case EBackward:
    {
        int pos = ui->pathLabel->text().lastIndexOf(pathDividerSign);
        int removeCount = labelTextlength - (pos + 1) + 1;

        if(pos == (discStrLength - 1)) ++pos;

        ui->pathLabel->setText(ui->pathLabel->text().remove(pos, removeCount));
    }
        break;
    }
}

int FilePanel::getDiscLength() const
{
    SettingsManager settingsManager;
    QString disc(settingsManager.accountDisc(settingsManager.currentAccount(panelNum)));

    disc += QString(":");
    disc += QDir::toNativeSeparators("/");

    return disc.length();
}



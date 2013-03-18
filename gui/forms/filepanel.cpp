#include "filepanel.h"
#include "ui_filepanel.h"
#include "share/defs.h"
#include "share/debug.h"
#include "gui/controls/spacer.h"
#include "settings/settingsmanager.h"
#include "core/driveengine.h"
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

void FilePanel::init(void)
{
    ui->setupUi(this);

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

    QStringList columnNames;
    columnNames << tr("Name")
                <<  tr("Size")
                #ifdef Q_OS_MAC
                 << tr("Kind")
                #else
                 << tr("Type")
                #endif
                 << "Date Modified";

    fileSystemModel.reset(new FileSystemModel(columnNames));
    computerDrivesView.reset(new QTreeView);
    ui->verticalLayout->addWidget(computerDrivesView.data());

    QFont font(ui->fileView->font());
    font.setBold(true);

    computerDrivesView->hide();

    computerDrivesView->setModel(fileSystemModel.data());
    computerDrivesView->setFont(font);
    computerDrivesView->setColumnWidth(0, 220);
    computerDrivesView->setSortingEnabled(true);
    computerDrivesView->sortByColumn(0, Qt::AscendingOrder);
    computerDrivesView->setRootIsDecorated(false);
    computerDrivesView->setCursor(Qt::PointingHandCursor);
    //computerDrivesView->setIconSize(QSize(32,32));

    connect(computerDrivesView.data(), SIGNAL(clicked(const QModelIndex&)), this, SLOT(slotComputerDrivesViewClicked(const QModelIndex&)));
    connect(computerDrivesView.data(), SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(slotComputerDrivesViewDoubleClicked(const QModelIndex&)));
    connect(accountsComboBox, SIGNAL(activated(const QString&)), SLOT(slotAccountsComboBoxActivated(const QString&)));
    connect(computerDrivesButton, SIGNAL(clicked()), SLOT(slotComputerDrivesButtonClicked()));
}

FilePanel::~FilePanel()
{
    delete ui;
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
        ui->fileView->hide();
        computerDrivesView->show();
    }
    else
    {
        ui->fileView->show();
        computerDrivesView->hide();
    }
}

void FilePanel::slotComputerDrivesViewClicked(const QModelIndex &index)
{
    DEBUG << fileSystemModel->filePath(index);
    insertBackToParentRow();
}

void FilePanel::slotComputerDrivesViewDoubleClicked(const QModelIndex &index)
{
    if(fileSystemModel->isDir(index))
        computerDrivesView->setRootIndex(fileSystemModel->index(fileSystemModel->filePath(index)));
}

void FilePanel::insertBackToParentRow()
{
    QModelIndex index = computerDrivesView->selectionModel()->currentIndex();
    QAbstractItemModel *model = computerDrivesView->model();

    if(!model->insertRow(index.row() + 1, index.parent()))
    {
        DEBUG << "!model->insertRow";
        DEBUG << "index.row()" << index.row();
        DEBUG << "index.parent()" << index.parent().parent();
        return;
    }

    for (int column = 0; column < model->columnCount(index.parent()); ++column)
    {
        QModelIndex child = model->index(index.row() + 1, column, index.parent());
        model->setData(child, QVariant("[No data]"), Qt::EditRole);
    }

 //   QDir dir("\\");
//    //dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
//    //dir.setSorting(QDir::Size | QDir::Reversed);

//    QFileInfoList list = dir.entryInfoList();
//     //QFileInfoList list = dir.drives();

//    QFileIconProvider fileIconProvider;

//    for (int i = 0; i < list.size(); ++i)
//    {
//        QFileInfo fileInfo = list.at(i);

//        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);

//        item->setText(0, fileInfo.fileName());
//        item->setIcon(0, fileIconProvider.icon(fileInfo));
//        item->setText(1, QString::number(fileInfo.size()));
//    }
}

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

    SDriveEngine::inst()->getContentMngr()->setPathesURLs(settingsManager.pathesURLs(panelNum));
    SDriveEngine::inst()->getContentMngr()->setPanel(ui->fileView, panelNum);
    SDriveEngine::inst()->getContentMngr()->get(settingsManager.currentFolderURL(panelNum));

    fillComboBox(settingsManager.accountsWithLetters(), accountName);
}


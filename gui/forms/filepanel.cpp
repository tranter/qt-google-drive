#include "filepanel.h"
#include "ui_filepanel.h"
#include "share/defs.h"
#include "share/debug.h"
#include "gui/controls/spacer.h"
#include "settings/settingsmanager.h"
#include "core/driveengine.h"
#include <QDir>
#include  <QApplication>
//#include  <QFontMetrics>

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
    //QFontMetrics accountsComboBoxFontMetrics(accountsComboBox->font());

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


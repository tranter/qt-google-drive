#include "filepanel.h"
#include "ui_filepanel.h"
#include "share/debug.h"
#include  <QApplication>

FilePanel::FilePanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilePanel)
{
    ui->setupUi(this);
    init();
}

void FilePanel::init(void)
{
    accountsToolBar = new ToolBar;
    ui->verticalLayout->insertWidget(0, accountsToolBar);

    accountsComboBox = new ComboBox;
    accountsToolBar->addWidget(accountsComboBox);
}

FilePanel::~FilePanel()
{
    delete ui;
}

QTreeWidget* FilePanel::getFileView(void) const
{
    return ui->fileView;
}

QLabel* FilePanel::getPathLabel(void) const
{
    return ui->pathLabel;
}

ToolBar* FilePanel::getAccountsToolBar(void) const
{
    return accountsToolBar;
}

ComboBox* FilePanel::getAccountsComboBox(void) const
{
    return accountsComboBox;
}

void FilePanel::fillComboBox(QMap<QString, QString> accountsMap, int index)
{
    QStringList keys(accountsMap.keys());

    for(int i = 0; i < keys.count(); ++i)
    {
        QString discLetter(keys[i]);

        discLetter = discLetter.leftJustified(4, ' ');

        accountsComboBox->addItem(discLetter + "[" + accountsMap[keys[i]] + "]");
        accountsComboBox->setItemIcon(i, QIcon(QApplication::style()->standardIcon(QStyle::QStyle::SP_DriveFDIcon)));
    }
}


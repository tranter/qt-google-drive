#include "filepanel.h"
#include "ui_filepanel.h"

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


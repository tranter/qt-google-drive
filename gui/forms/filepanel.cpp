#include "filepanel.h"
#include "ui_filepanel.h"
#include "gui/controls/toolbar.h"
#include "gui/controls/combobox.h"

FilePanel::FilePanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilePanel)
{
    ui->setupUi(this);

    ToolBar *toolBar = new ToolBar;
    ui->verticalLayout->insertWidget(0, toolBar);

    ComboBox *comboBox = new ComboBox;
    toolBar->addWidget(comboBox);
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


#include "filepanel.h"
#include "ui_filepanel.h"

FilePanel::FilePanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilePanel)
{
    ui->setupUi(this);
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

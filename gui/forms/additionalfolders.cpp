#include "additionalfolders.h"
#include "ui_additionalfolders.h"

AdditionalFolders::AdditionalFolders(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdditionalFolders)
{
    ui->setupUi(this);
}

AdditionalFolders::~AdditionalFolders()
{
    delete ui;
}

#include "drivetree.h"
#include "ui_drivetree.h"

DriveTree::DriveTree(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DriveTree)
{
    ui->setupUi(this);
}

DriveTree::~DriveTree()
{
    delete ui;
}

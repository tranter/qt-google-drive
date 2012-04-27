#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    driveEngine(NULL)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(driveEngine) delete driveEngine;
}

void MainWindow::init(void)
{
    DriveEngine* driveEngine = new DriveEngine(this);
    driveEngine->init();

    connect(ui->actionLogin, SIGNAL(triggered()), driveEngine,  SLOT(slotStartLogin()));
    connect(ui->actionQuit, SIGNAL(triggered()), this,  SLOT(close()));
}

void MainWindow::on_getButton_clicked()
{
    emit getButtonClick();
}

void MainWindow::on_postButton_clicked()
{
   emit postButtonClick();
}

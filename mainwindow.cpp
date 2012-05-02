#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

//Ui::MainWindow* MainWindow::ui = NULL;

Ui::MainWindow* UiInstance::ui = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    driveEngine(NULL)
{
    UiInstance::Instance()->setupUi(this);
}

MainWindow::~MainWindow()
{
    //delete ui;
    delete UiInstance::ui;
    if(driveEngine) delete driveEngine;
}

void MainWindow::init(void)
{
    DriveEngine* driveEngine = new DriveEngine(this);
    driveEngine->init();

//    connect(ui->actionLogin, SIGNAL(triggered()), driveEngine,  SLOT(slotStartLogin()));
//    connect(ui->actionQuit, SIGNAL(triggered()), this,  SLOT(close()));

    connect(UiInstance::ui->actionLogin, SIGNAL(triggered()), driveEngine,  SLOT(slotStartLogin()));
    connect(UiInstance::ui->actionQuit, SIGNAL(triggered()), this,  SLOT(close()));
}

void MainWindow::on_getButton_clicked()
{
    emit getButtonClick();
}

void MainWindow::on_postButton_clicked()
{
    emit postButtonClick();
}

//Ui::MainWindow* UiInstance::Instance()
//{
//    if(!MainWindow::ui)
//        MainWindow::ui = new Ui::MainWindow;

//    return MainWindow::ui;
//}

Ui::MainWindow* UiInstance::Instance()
{
    if(!UiInstance::ui)
        UiInstance::ui = new Ui::MainWindow;

    return UiInstance::ui;
}

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    driveEngine(NULL)
{
    ui->setupUi(this);

    DriveEngine* driveEngine = new DriveEngine();

    connect(ui->actionLogin, SIGNAL(triggered()), driveEngine,  SLOT(startLogin()));
    connect(ui->actionQuit, SIGNAL(triggered()), this,  SLOT(close()));
}

MainWindow::~MainWindow()
{
    delete ui;
    if(driveEngine) delete driveEngine;
}

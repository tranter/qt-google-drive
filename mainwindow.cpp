#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>

Ui::MainWindow* UiInstance::ui = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    driveEngine(NULL)
{
    UiInstance::Instance()->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete UiInstance::ui;
    if(driveEngine) delete driveEngine;
}

void MainWindow::init(void)
{
    if(driveEngine)
    {
        delete driveEngine;
        driveEngine = NULL;
    }

    driveEngine = new DriveEngine(this);
    driveEngine->init();

    connect(UiInstance::ui->actionLogin, SIGNAL(triggered()), driveEngine, SLOT(slotStartLogin()));
    connect(UiInstance::ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(driveEngine->getOAuth2(), SIGNAL(loginDone()), this, SLOT(slotloginDone()));
    connect(UiInstance::ui->downloadButton, SIGNAL(clicked()), driveEngine, SLOT(slotDownload()));
    connect(UiInstance::ui->uploadButton, SIGNAL(clicked()), driveEngine, SLOT(slotUpload()));
    connect(UiInstance::ui->actionSettings, SIGNAL(triggered()), driveEngine, SLOT(slotCheckWorkDir()));
    connect(driveEngine, SIGNAL(signalUploadFinished()), this, SLOT(slotUpdateModel()));

    driveEngine->slotCheckWorkDir(false);

    emit siganalGet();
}

Ui::MainWindow* UiInstance::Instance()
{
    if(!UiInstance::ui)
        UiInstance::ui = new Ui::MainWindow;

    return UiInstance::ui;
}

void MainWindow::slotloginDone(void)
{
    init();
}

void MainWindow::slotUpdateModel()
{
  qDebug() << "slotUpdateModel()";
  init();
}


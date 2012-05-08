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

    connect(UiInstance::ui->actionLogin, SIGNAL(triggered()), driveEngine,  SLOT(slotStartLogin()));
    connect(UiInstance::ui->actionQuit, SIGNAL(triggered()), this,  SLOT(close()));
    connect(driveEngine->getOAuth2(), SIGNAL(loginDone()), this,  SLOT(slotloginDone()));
    connect(UiInstance::ui->downloadButton, SIGNAL(clicked()), driveEngine,  SLOT(slotDownload()));
    connect(UiInstance::ui->actionSettings, SIGNAL(triggered()), this,  SLOT(slotCheckWorkDir()));

    slotCheckWorkDir(false);

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

void MainWindow::slotCheckWorkDir(bool showDlg)
{
    QSettings settings(COMPANY_NAME, APP_NAME);
    SettingsDialog dlg(this);

    if(settings.contains(WORK_DIR) && showDlg)
    {
        dlg.setDirectoryPath(settings.value(WORK_DIR).toString());
    }
    else if(settings.contains(WORK_DIR) && !showDlg)
    {
       return;
    }

    switch(dlg.exec())
    {
    case QDialog::Accepted:
    {
        qDebug() << "QDialog::Accepted";
        if(dlg.directoryPath() != "" )
           settings.setValue(WORK_DIR,dlg.directoryPath());
    }
        break;
    }
}

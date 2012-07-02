#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include "AppRegData.h"
#include <QKeyEvent>

Ui::MainWindow* UiInstance::ui = NULL;
DriveEngine* DriveEngineInstance::driveEngine = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    driveEngine(NULL)
{
    UiInstance::Instance()->setupUi(this);
}

MainWindow::~MainWindow()
{
     if(driveEngine) delete driveEngine;
}

void MainWindow::init(void)
{
    qDebug() << "MainWindow::init";

    if(!CheckReg()) return;

    if(driveEngine) delete driveEngine;
    driveEngine = DriveEngineInstance::Instance();
    driveEngine->init();
    driveEngine->slotCheckWorkDir(false);

    setConnections();

    UiInstance::Instance()->foldersView->installEventFilter(this);
    UiInstance::Instance()->filesView->installEventFilter(this);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
}

void MainWindow::setConnections(void)
{
    connect(UiInstance::Instance()->actionMenuLogin, SIGNAL(triggered()), driveEngine, SLOT(slotStartLoginFromMenu()));
    connect(UiInstance::Instance()->actionMenuQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(UiInstance::Instance()->actionMenuDownload, SIGNAL(triggered()), driveEngine, SLOT(slotDownload()));
    connect(UiInstance::Instance()->actionDownload, SIGNAL(triggered()), driveEngine, SLOT(slotDownload()));
    connect(UiInstance::Instance()->actionMenuUpload, SIGNAL(triggered()), driveEngine, SLOT(slotUpload()));
    connect(UiInstance::Instance()->actionUpload, SIGNAL(triggered()), driveEngine, SLOT(slotUpload()));
    connect(UiInstance::Instance()->actionMenuSettings, SIGNAL(triggered()), driveEngine, SLOT(slotCheckWorkDir()));
    connect(UiInstance::Instance()->actionSettings, SIGNAL(triggered()), driveEngine, SLOT(slotCheckWorkDir()));
    connect(driveEngine->getOAuth2(), SIGNAL(loginDone()), this, SLOT(slotloginDone()));
    connect(driveEngine->getFoldersManager(), SIGNAL(signalAccessTokenRequired()), driveEngine, SLOT(slotStartLogin()));
    connect(this, SIGNAL(signalDel(QObject*)), driveEngine, SLOT(slotDel(QObject*)));
    connect(UiInstance::Instance()->actionMenuDelete, SIGNAL(triggered()), driveEngine, SLOT(slotTriggeredDel()));
    connect(UiInstance::Instance()->actionDelete, SIGNAL(triggered()), driveEngine, SLOT(slotTriggeredDel()));
    connect(UiInstance::Instance()->actionMenuCreateFolder, SIGNAL(triggered()), driveEngine, SLOT(slotCreateFolder()));
    connect(UiInstance::Instance()->actionCreateFolder, SIGNAL(triggered()), driveEngine, SLOT(slotCreateFolder()));
}

Ui::MainWindow* UiInstance::Instance()
{
    if(!ui) ui = new Ui::MainWindow;
    return ui;
}

DriveEngine* DriveEngineInstance::Instance()
{
    if(!driveEngine) driveEngine = new DriveEngine;
    return driveEngine;
}

void MainWindow::slotloginDone()
{
    init();
}

void MainWindow::slotUpdateFileList()
{
    qDebug("MainWindow::slotUpdateFileList");
    driveEngine->showFiles();
}

bool MainWindow::CheckReg(void)
{
    bool regState = true;

    if(CLIENT_ID == "YOUR_CLIENT_ID_HERE" || REDIRECT_URI == "YOUR_REDIRECT_URI_HERE" || CLIENT_SECRET == "YOUR_CLIENT_SECRET")
    {
        regState = false;

        // TODO: change link to wiki page
        QMessageBox::warning(this, "Warning",
                             "To work with application you need to register your own application in <b>Google</b>.\n"
                             "Learn more from <a href='http://code.google.com/p/qt-google-drive/'>here</a>");
    }

    return regState;
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

        if (keyEvent->key() == Qt::Key_Delete) {
            emit signalDel(object);
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}

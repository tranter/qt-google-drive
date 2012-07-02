#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include "AppRegData.h"
#include <QKeyEvent>

Ui::MainWindow* SUi::ui = NULL;
DriveEngine* SDriveEngine::driveEngine = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    SUi::Inst()->setupUi(this);
}

MainWindow::~MainWindow()
{
    SDriveEngine::FreeInst();
    SUi::FreeInst();
}

void MainWindow::init(void)
{
    qDebug() << "MainWindow::init";

    if(!CheckReg()) return;

    SDriveEngine::FreeInst();
    SDriveEngine::Inst()->init();
    SDriveEngine::Inst()->slotCheckWorkDir(false);

    setConnections();

    SUi::Inst()->foldersView->installEventFilter(this);
    SUi::Inst()->filesView->installEventFilter(this);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
}

void MainWindow::setConnections(void)
{
    connect(SUi::Inst()->actionMenuLogin, SIGNAL(triggered()), SDriveEngine::Inst(), SLOT(slotStartLoginFromMenu()));
    connect(SUi::Inst()->actionMenuQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(SUi::Inst()->actionMenuDownload, SIGNAL(triggered()), SDriveEngine::Inst(), SLOT(slotDownload()));
    connect(SUi::Inst()->actionDownload, SIGNAL(triggered()), SDriveEngine::Inst(), SLOT(slotDownload()));
    connect(SUi::Inst()->actionMenuUpload, SIGNAL(triggered()), SDriveEngine::Inst(), SLOT(slotUpload()));
    connect(SUi::Inst()->actionUpload, SIGNAL(triggered()), SDriveEngine::Inst(), SLOT(slotUpload()));
    connect(SUi::Inst()->actionMenuSettings, SIGNAL(triggered()), SDriveEngine::Inst(), SLOT(slotCheckWorkDir()));
    connect(SUi::Inst()->actionSettings, SIGNAL(triggered()), SDriveEngine::Inst(), SLOT(slotCheckWorkDir()));
    connect(SDriveEngine::Inst()->getOAuth2(), SIGNAL(loginDone()), this, SLOT(slotloginDone()));
    connect(SDriveEngine::Inst()->getFoldersManager(), SIGNAL(signalAccessTokenRequired()), SDriveEngine::Inst(), SLOT(slotStartLogin()));
    connect(this, SIGNAL(signalDel(QObject*)), SDriveEngine::Inst(), SLOT(slotDel(QObject*)));
    connect(SUi::Inst()->actionMenuDelete, SIGNAL(triggered()), SDriveEngine::Inst(), SLOT(slotTriggeredDel()));
    connect(SUi::Inst()->actionDelete, SIGNAL(triggered()), SDriveEngine::Inst(), SLOT(slotTriggeredDel()));
    connect(SUi::Inst()->actionMenuCreateFolder, SIGNAL(triggered()), SDriveEngine::Inst(), SLOT(slotCreateFolder()));
    connect(SUi::Inst()->actionCreateFolder, SIGNAL(triggered()), SDriveEngine::Inst(), SLOT(slotCreateFolder()));
}

Ui::MainWindow* SUi::Inst()
{
    if(!ui) ui = new Ui::MainWindow;
    return ui;
}

DriveEngine* SDriveEngine::Inst()
{
    if(!driveEngine) driveEngine = new DriveEngine;
    return driveEngine;
}

void MainWindow::slotloginDone()
{
    init();
}

//void MainWindow::slotUpdateFileList()
//{
//    qDebug("MainWindow::slotUpdateFileList");
//    SDriveEngine::Inst()->showFiles();
//}

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

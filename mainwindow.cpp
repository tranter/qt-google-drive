#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QMessageBox>
#include "AppRegData.h"
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    SUi::inst()->setupUi(this);
}

MainWindow::~MainWindow()
{
    SDriveEngine::freeInst();
    SUi::freeInst();
}

void MainWindow::init(void)
{
    if(!checkReg()) return;

    SDriveEngine::freeInst();
    SDriveEngine::inst(this)->init();
    SDriveEngine::inst()->slotCheckWorkDir(false);

    setConnections();

    SUi::inst()->foldersView->installEventFilter(this);
    SUi::inst()->filesView->installEventFilter(this);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
}

void MainWindow::setConnections(void)
{
    connect(SUi::inst()->actionMenuLogin, SIGNAL(triggered()), SDriveEngine::inst(), SLOT(slotStartLoginFromMenu()));
    connect(SUi::inst()->actionMenuQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(SUi::inst()->actionMenuDownload, SIGNAL(triggered()), SDriveEngine::inst()->getfilesTransferUI(), SLOT(slotDownload()));
    connect(SUi::inst()->actionDownload, SIGNAL(triggered()), SDriveEngine::inst()->getfilesTransferUI(), SLOT(slotDownload()));
    connect(SUi::inst()->actionMenuUpload, SIGNAL(triggered()), SDriveEngine::inst()->getfilesTransferUI(), SLOT(slotUpload()));
    connect(SUi::inst()->actionUpload, SIGNAL(triggered()), SDriveEngine::inst()->getfilesTransferUI(), SLOT(slotUpload()));
    connect(SUi::inst()->actionMenuSettings, SIGNAL(triggered()), SDriveEngine::inst(), SLOT(slotCheckWorkDir()));
    connect(SUi::inst()->actionSettings, SIGNAL(triggered()), SDriveEngine::inst(), SLOT(slotCheckWorkDir()));
    connect(SDriveEngine::inst()->getOAuth2(), SIGNAL(loginDone()), this, SLOT(slotloginDone()));
    connect(SDriveEngine::inst()->getFoldersManager(), SIGNAL(signalAccessTokenRequired()), SDriveEngine::inst(), SLOT(slotStartLogin())); 
    connect(this, SIGNAL(signalDel(QObject*)), SDriveEngine::inst()->getOperationsUI(), SLOT(slotDel(QObject*)));
    connect(SUi::inst()->actionMenuDelete, SIGNAL(triggered()), SDriveEngine::inst()->getOperationsUI(), SLOT(slotTriggeredDel()));
    connect(SUi::inst()->actionDelete, SIGNAL(triggered()), SDriveEngine::inst()->getOperationsUI(), SLOT(slotTriggeredDel()));
    connect(SUi::inst()->actionMenuCreateFolder, SIGNAL(triggered()), SDriveEngine::inst()->getOperationsUI(), SLOT(slotCreateFolder()));
    connect(SUi::inst()->actionCreateFolder, SIGNAL(triggered()), SDriveEngine::inst()->getOperationsUI(), SLOT(slotCreateFolder()));
}

void MainWindow::slotloginDone()
{
    init();
}

bool MainWindow::checkReg(void)
{
    bool regState = true;

    if(CLIENT_ID == QString("YOUR_CLIENT_ID_HERE") || REDIRECT_URI == QString("YOUR_REDIRECT_URI_HERE") || CLIENT_SECRET == QString("YOUR_CLIENT_SECRET"))
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

        if (keyEvent->key() == Qt::Key_Delete)
        {
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

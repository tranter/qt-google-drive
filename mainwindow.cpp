#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "driveengine.h"
#include <QTextCodec>
#include <QKeyEvent>
#include <QDebug>

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
    {
        CheckUI checkUI;
        if(!checkUI.checkReg()) return;
    }

    SDriveEngine::freeInst();
    SDriveEngine::inst(this)->init();
    SDriveEngine::inst()->getCheckUI()->slotCheckWorkDir(false);

    setConnections();

    SUi::inst()->treeFoldersView->installEventFilter(this);
    SUi::inst()->filesViewRight->installEventFilter(this);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    //SUi::inst()->filesViewRight->header()->resizeSection(0, 360);
    //SUi::inst()->treeFoldersView->setVisible(false);
    //SUi::inst()->aFoldersView->setVisible(false);
    //SUi::inst()->filesViewRight->header()->setCursor(Qt::SizeHorCursor);

    SUi::inst()->filesViewLeft->setVisible(false);
}

void MainWindow::setConnections(void)
{
    connect(SUi::inst()->actionMenuLogin, SIGNAL(triggered()), SDriveEngine::inst(), SLOT(slotStartLoginFromMenu()));
    connect(SUi::inst()->actionMenuQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(SUi::inst()->actionMenuDownload, SIGNAL(triggered()), SDriveEngine::inst()->getfilesTransferUI(), SLOT(slotDownload()));
    connect(SUi::inst()->actionDownload, SIGNAL(triggered()), SDriveEngine::inst()->getfilesTransferUI(), SLOT(slotDownload()));
    connect(SUi::inst()->actionMenuUpload, SIGNAL(triggered()), SDriveEngine::inst()->getfilesTransferUI(), SLOT(slotUpload()));
    connect(SUi::inst()->actionUpload, SIGNAL(triggered()), SDriveEngine::inst()->getfilesTransferUI(), SLOT(slotUpload()));
    connect(SUi::inst()->actionMenuSettings, SIGNAL(triggered()), SDriveEngine::inst()->getCheckUI(), SLOT(slotCheckWorkDir()));
    connect(SUi::inst()->actionSettings, SIGNAL(triggered()), SDriveEngine::inst()->getCheckUI(), SLOT(slotCheckWorkDir()));
    connect(SUi::inst()->actionMenuDelete, SIGNAL(triggered()), SDriveEngine::inst()->getOpUI(), SLOT(slotTriggeredDel()));
    connect(SUi::inst()->actionDelete, SIGNAL(triggered()), SDriveEngine::inst()->getOpUI(), SLOT(slotTriggeredDel()));
    connect(SUi::inst()->actionMenuCreateFolder, SIGNAL(triggered()), SDriveEngine::inst()->getOpUI(), SLOT(slotCreateFolder()));
    connect(SUi::inst()->actionCreateFolder, SIGNAL(triggered()), SDriveEngine::inst()->getOpUI(), SLOT(slotCreateFolder()));
    connect(SUi::inst()->treeFoldersView, SIGNAL(clicked (const QModelIndex&)), SDriveEngine::inst()->getFoldersUI(), SLOT(slotFoldersViewClicked(const QModelIndex&)));
    connect(SUi::inst()->filesViewRight, SIGNAL(clicked (const QModelIndex&)), SDriveEngine::inst()->getfilesUI(), SLOT(slotFilesViewClicked(const QModelIndex&)));
    connect(SUi::inst()->aFoldersView, SIGNAL(clicked (const QModelIndex&)), SDriveEngine::inst()->getfilesUI(), SLOT(slotAShowFiles(const QModelIndex&)));
    connect(SUi::inst()->filesViewRight->header(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), SDriveEngine::inst()->getfilesUI(), SLOT(slotFilesSortIndicatorChanged(int, Qt::SortOrder)));
    connect(SDriveEngine::inst()->getFoldersMngr(), SIGNAL(signalAccessTokenRequired()), SDriveEngine::inst(), SLOT(slotStartLogin()));
    connect(SDriveEngine::inst()->getOAuth2(), SIGNAL(loginDone()), this, SLOT(slotloginDone()));
    connect(SDriveEngine::inst()->getFoldersMngr(), SIGNAL(signalAccessTokenRequired()), SDriveEngine::inst(), SLOT(slotStartLogin()));
}

void MainWindow::slotloginDone()
{
    init();
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    return SDriveEngine::inst()->getEventHandler()->event(object, event);
}



#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "driveengine.h"
#include <QTextCodec>
#include <QKeyEvent>
#include <QToolButton>
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
    SUi::inst()->filesViewLeft->installEventFilter(this);
    SUi::inst()->filesViewRight->installEventFilter(this);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    SUi::inst()->filesViewLeft->header()->setClickable(true);
    SUi::inst()->filesViewRight->header()->setClickable(true);

    SUi::inst()->filesViewLeft->header()->resizeSection(0, 250);
    SUi::inst()->filesViewRight->header()->resizeSection(0, 250);

    SUi::inst()->treeFoldersView->setVisible(false);
    SUi::inst()->aFoldersView->setVisible(false);

//    QToolButton* tb = new QToolButton();
//    tb->setText("a");
//    tb->setMinimumWidth(32);
//    tb->setMinimumHeight(32);
//    SUi::inst()->discsToolBar->addWidget(tb);

//    SUi::inst()->statusBar->showMessage("");
//    SUi::inst()->mainToolBar->setIconSize(QSize(48,48));
}

void MainWindow::setConnections(void)
{
    connect(SUi::inst()->actionLogin, SIGNAL(triggered()), SDriveEngine::inst(), SLOT(slotStartLoginFromMenu()));
    connect(SUi::inst()->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(SUi::inst()->actionDownload, SIGNAL(triggered()), SDriveEngine::inst()->getfilesTransferUI(), SLOT(slotDownload()));
    connect(SUi::inst()->actionUpload, SIGNAL(triggered()), SDriveEngine::inst()->getfilesTransferUI(), SLOT(slotUpload()));
    connect(SUi::inst()->actionSettings, SIGNAL(triggered()), SDriveEngine::inst()->getCheckUI(), SLOT(slotCheckWorkDir()));
    connect(SUi::inst()->actionDelete, SIGNAL(triggered()), SDriveEngine::inst()->getOpUI(), SLOT(slotTriggeredDel()));
    connect(SUi::inst()->actionNewFolder, SIGNAL(triggered()), SDriveEngine::inst()->getOpUI(), SLOT(slotNewFolder()));
    connect(SUi::inst()->copyButton, SIGNAL(clicked()), this, SLOT(slotStub()));
    connect(SUi::inst()->moveButton, SIGNAL(clicked()), this, SLOT(slotStub()));
    connect(SUi::inst()->newFolderButton, SIGNAL(clicked()), SDriveEngine::inst()->getOpUI(), SLOT(slotNewFolder()));
    connect(SUi::inst()->deleteButton, SIGNAL(clicked()), SDriveEngine::inst()->getOpUI(), SLOT(slotTriggeredDel()));
    connect(SUi::inst()->renameButton, SIGNAL(clicked()), this, SLOT(slotStub()));
    connect(SUi::inst()->shareButton, SIGNAL(clicked()), this, SLOT(slotStub()));
    connect(SUi::inst()->treeFoldersView, SIGNAL(clicked (const QModelIndex&)), SDriveEngine::inst()->getFoldersUI(), SLOT(slotFoldersViewClicked(const QModelIndex&)));
    connect(SUi::inst()->filesViewLeft, SIGNAL(clicked(const QModelIndex&)), SDriveEngine::inst()->getfilesUI(), SLOT(slotLeftViewClicked(const QModelIndex&)));
    connect(SUi::inst()->filesViewRight, SIGNAL(clicked(const QModelIndex&)), SDriveEngine::inst()->getfilesUI(), SLOT(slotRightViewClicked(const QModelIndex&)));
    connect(SUi::inst()->aFoldersView, SIGNAL(clicked (const QModelIndex&)), SDriveEngine::inst()->getfilesUI(), SLOT(slotAShowFiles(const QModelIndex&)));
    connect(SUi::inst()->filesViewLeft->header(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), SDriveEngine::inst()->getfilesUI(), SLOT(slotLeftSortIndicatorChanged(int, Qt::SortOrder)));
    connect(SUi::inst()->filesViewRight->header(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), SDriveEngine::inst()->getfilesUI(), SLOT(slotRightSortIndicatorChanged(int, Qt::SortOrder)));
    connect(SDriveEngine::inst()->getFoldersMngr()->self(), SIGNAL(signalAccessTokenRequired()), SDriveEngine::inst(), SLOT(slotStartLogin()));
    connect(SDriveEngine::inst()->getOAuth2(), SIGNAL(loginDone()), this, SLOT(slotloginDone()));
    connect(SDriveEngine::inst()->getCurrFilesMngr(), SIGNAL(signalAccessTokenRequired()), SDriveEngine::inst(), SLOT(slotStartLogin()));
    connect(SDriveEngine::inst()->getCurrFilesMngr(), SIGNAL(signalFirstPanelIsLoaded()), SDriveEngine::inst(), SLOT(slotFirstPanelIsLoaded()));
}

void MainWindow::slotloginDone()
{
    init();
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    return SDriveEngine::inst()->getEventHandler()->event(object, event);
}

void MainWindow::paintEvent(QPaintEvent*)
{
  SUi::inst()->pathLabelLeft->setMaximumWidth(SUi::inst()->filesViewLeft->width());
  SUi::inst()->pathLabelRight->setMaximumWidth(SUi::inst()->filesViewRight->width());
}

void MainWindow::slotStub(void)
{
    CommonTools::msg("Not implemented yet");
}




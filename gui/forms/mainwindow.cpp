#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core/driveengine.h"
#include "share/debug.h"
#include "settings/settingsmanager.h"
#include "gui/forms/authdialog.h"
#include "network/operations/operationsmanager.h"
#include <QTextCodec>
#include <QKeyEvent>
#include <QToolButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    auth(NULL)
{
    SUi::inst()->setupUi(this);
}

MainWindow::~MainWindow()
{
    SDriveEngine::freeInst();
    SUi::freeInst();
    SOperationsManager::freeInst();
}

void MainWindow::init(void)
{
    SDriveEngine::inst(this)->init();
    SDriveEngine::inst()->getCheckUI()->slotCheckWorkDir(false);

    setConnections();

    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    if(!SettingsManager().isAnyAccount()) slotAuthDialog();

    //    if(!CheckUI().checkReg())
    //    {
    //        CommonTools::msg(tr("The application is under development. Currently disabled to use.\nNo commercial use allowed."));
    //        return;
    //    }
}

void MainWindow::setConnections(void)
{
    connect(SUi::inst()->actionLogin, SIGNAL(triggered()), this, SLOT(slotAuthDialog()));
    connect(SUi::inst()->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(SUi::inst()->actionDownload, SIGNAL(triggered()), SDriveEngine::inst()->getfilesTransferUI(), SLOT(slotDownload()));
    connect(SUi::inst()->actionUpload, SIGNAL(triggered()), SDriveEngine::inst()->getfilesTransferUI(), SLOT(slotUpload()));
    connect(SUi::inst()->actionSettings, SIGNAL(triggered()), SDriveEngine::inst()->getCheckUI(), SLOT(slotCheckWorkDir()));
    connect(SUi::inst()->copyButton, SIGNAL(clicked()), SOperationsManager::inst(), SLOT(slotCopy()));
    connect(SUi::inst()->moveButton, SIGNAL(clicked()), SOperationsManager::inst(), SLOT(slotMove()));
    connect(SUi::inst()->newFolderButton, SIGNAL(clicked()), SOperationsManager::inst(), SLOT(slotNewFolder()));
    connect(SUi::inst()->deleteButton, SIGNAL(clicked()), SOperationsManager::inst(), SLOT(slotDelete()));
    connect(SUi::inst()->renameButton, SIGNAL(clicked()), SOperationsManager::inst(), SLOT(slotRename()));
    connect(SUi::inst()->shareButton, SIGNAL(clicked()), SOperationsManager::inst(), SLOT(slotShare()));
    connect(SDriveEngine::inst()->getFilePanel(ELeft)->getFileView(), SIGNAL(itemPressed(QTreeWidgetItem*, int)), SDriveEngine::inst()->getContentUI(), SLOT(slotItemLeftPressed(QTreeWidgetItem*, int)));
    connect(SDriveEngine::inst()->getFilePanel(ERight)->getFileView(), SIGNAL(itemPressed(QTreeWidgetItem*, int)), SDriveEngine::inst()->getContentUI(), SLOT(slotItemRightPressed(QTreeWidgetItem*, int)));
    connect(SDriveEngine::inst()->getFilePanel(ELeft)->getFileView(), SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), SDriveEngine::inst()->getContentUI(), SLOT(slotLeftPanelItemDoubleClicked(QTreeWidgetItem*, int)));
    connect(SDriveEngine::inst()->getFilePanel(ERight)->getFileView(), SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), SDriveEngine::inst()->getContentUI(), SLOT(slotRightPanelItemDoubleClicked(QTreeWidgetItem*, int)));
    connect(SDriveEngine::inst()->getFilePanel(ELeft)->getFileView(), SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), SDriveEngine::inst()->getContentUI(), SLOT(slotLeftCurrentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
    connect(SDriveEngine::inst()->getFilePanel(ERight)->getFileView(), SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), SDriveEngine::inst()->getContentUI(), SLOT(slotRightCurrentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
    connect(SDriveEngine::inst()->getFilePanel(ELeft)->getFileView(), SIGNAL(itemEntered(QTreeWidgetItem*, int)), SDriveEngine::inst()->getContentUI(), SLOT(slotLeftItemEntered(QTreeWidgetItem*, int)));
    connect(SDriveEngine::inst()->getFilePanel(ERight)->getFileView(), SIGNAL(itemEntered(QTreeWidgetItem*, int)), SDriveEngine::inst()->getContentUI(), SLOT(slotRightItemEntered(QTreeWidgetItem*, int)));
    connect(SDriveEngine::inst()->getContentMngr()->self(), SIGNAL(signalAccessTokenRequired()), this, SLOT(slotAccessTokenRequired()));
    connect(SDriveEngine::inst()->getContentMngr(true)->self(), SIGNAL(signalAccessTokenRequired()), this, SLOT(slotAccessTokenRequired()));
    connect(SDriveEngine::inst()->getContentMngr(), SIGNAL(signalFirstPanelIsLoaded()), SDriveEngine::inst(), SLOT(slotFirstPanelIsLoaded()));
    connect(SOperationsManager::inst(), SIGNAL(signalAccountInfoReadyToUse()), this, SLOT(slotAccountInfoReadyToUse()));
    connect(SUi::inst()->actionUpdate, SIGNAL(triggered()), this, SLOT(slotStub()));
    connect(SUi::inst()->actionRecycleBin, SIGNAL(triggered()), this, SLOT(slotStub()));
    connect(SUi::inst()->actionSend, SIGNAL(triggered()), this, SLOT(slotStub()));
    connect(SUi::inst()->actionSearch, SIGNAL(triggered()), this, SLOT(slotStub()));
    connect(SUi::inst()->actionAbout, SIGNAL(triggered()), this, SLOT(slotAbout()));
}

void MainWindow::slotAuthDialog(void)
{
    AuthDialog authDialog(SUi::inst()->centralWidget);
    connect(&authDialog, SIGNAL(signalTokens(const QString&, const QString&)), this, SLOT(slotTokens(const QString&, const QString&)));
    authDialog.exec();
}

void MainWindow::slotTokens(const QString &accessToken, const QString &refreshToken)
{
    SOperationsManager::inst()->setAccountInfo(accessToken, refreshToken);
}

void MainWindow::slotStub()
{
    CommonTools::msg("Under re-construction with new API version. Will be soon.");
}

void MainWindow::slotAbout()
{
    CommonTools::msg("Google Drive Web File Manager\n\nIntegrated Computer Solutions, Inc.");
}

void MainWindow::slotAccountInfoReadyToUse(void)
{
    SDriveEngine::inst()->updatePanel(ELeft, true);
}

void MainWindow::slotAccessTokenRequired(void)
{
    SettingsManager settingsManager;

    if(auth) delete auth;
    auth = new Auth;

    auth->getAccessToken(settingsManager.clientId(), settingsManager.clientSecret(), settingsManager.refreshToken());
    connect(auth, SIGNAL(signalAuthResponse(const QString&)), this, SLOT(slotAuthResponse(const QString&)));
}

void MainWindow::slotAuthResponse(const QString &accessToken)
{
    SOperationsManager::inst()->setAccountInfo(accessToken);

    auth->deleteLater();
    auth = NULL;
}



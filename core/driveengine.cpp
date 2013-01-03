#include "driveengine.h"
#include "share/debug.h"
#include "settings/settingsmanager.h"
#include <QMessageBox>
#include <QHBoxLayout>
#include <QSplitter>
#include <QDir>

DriveEngine::DriveEngine(QWidget *p) :
    parent(p)
{
}

DriveEngine::~DriveEngine()
{
}

void DriveEngine::init(void)
{
    reset();

    if(SettingsManager().isAnyAccount())
    {
        updatePanel(LEFT_PANEL_VALUE, true);
    }
}

void DriveEngine::reset(void)
{
    QHBoxLayout *hBoxLayout = new QHBoxLayout(SUi::inst()->panelsWidget);
    QSplitter *hSplitter = new QSplitter(Qt::Horizontal, SUi::inst()->panelsWidget);

    filesViews[ELeft] = new FilePanel(ELeft);
    filesViews[ERight] = new FilePanel(ERight);

    hBoxLayout->setContentsMargins(0, 0, 0, 0);
    hBoxLayout->addWidget(hSplitter);

    hSplitter->setHandleWidth(1);

    hSplitter->addWidget(filesViews[ELeft]);
    hSplitter->addWidget(filesViews[ERight]);

    for(int i = 0; i < EPanelsCount; ++i)
    {
        filesMngr[i].reset(new FilesManager);
        filesMngr[i]->init();
    }

    checkUI.reset(new CheckUI);
    filesTransferUI.reset(new FilesTransferUI);
    filesUI.reset(new FilesUI);
    foldersMngr.reset(new FoldersManager);
    foldersUI.reset(new FoldersUI);

    connect(filesViews[ELeft], SIGNAL(signalAccountChanged(int, const QString&)), SLOT(slotAccountChanged(int, const QString&)));
    connect(filesViews[ERight], SIGNAL(signalAccountChanged(int, const QString&)), SLOT(slotAccountChanged(int, const QString&)));
}

FilePanel* DriveEngine::getFilePanel(EPanels panel) const
{
    return filesViews[panel];
}

FilesManager* DriveEngine::getFilesMngr(bool opposite) const
{
    FilesManager* filesManager;
    EPanels currentPanel = static_cast<EPanels> (SettingsManager().currentPanel());

    if(currentPanel == ELeft)
    {
        if(opposite) filesManager = filesMngr[ERight].data();
        else filesManager = filesMngr[ELeft].data();
    }

    if(currentPanel == ERight)
    {
        if(opposite) filesManager = filesMngr[ELeft].data();
        else filesManager = filesMngr[ERight].data();
    }

    return filesManager;
}

void DriveEngine::updatePanel(int panelNum, bool initLoad)
{
    SettingsManager settingsManager;
    EPanels panelId = static_cast <EPanels> (panelNum);
    QString drive;

    settingsManager.setInitialLoading(initLoad);
    settingsManager.setCurrentPanel(panelNum);

    drive = settingsManager.accountLetter(settingsManager.currentAccount(panelNum));
    drive += QString(":");
    drive += QDir::toNativeSeparators("/");

    filesUI->getPanelLabel(panelId)->setText(drive + settingsManager.currentFolderPath(panelNum));
    getFilesMngr()->setPathesURLs(settingsManager.pathesURLs(panelNum));

    filesMngr[panelNum]->setPanel(filesViews[panelNum]->getFileView(), filesViews[panelNum]->getpanelNum());
    filesMngr[panelNum]->get(settingsManager.currentFolderURL(panelNum));

    getFilePanel(panelId)->fillComboBox(settingsManager.accountsWithLetters(), settingsManager.currentAccount(panelNum));
}

void DriveEngine::slotFirstPanelIsLoaded(void)
{
    updatePanel(RIGHT_PANEL_VALUE, false);
}

void DriveEngine::slotAccountChanged(int panelNum, const QString &accountName)
{
    SettingsManager settingsManager;

    if(settingsManager.currentAccount(panelNum) != accountName)
    {
        settingsManager.setCurrentAccount(panelNum, accountName);
        updatePanel(panelNum, false);
    }
}

CheckUI* DriveEngine::getCheckUI(void) const
{
    return checkUI.data();
}

FilesUI* DriveEngine::getfilesUI(void) const
{
    return filesUI.data();
}

FilesTransferUI* DriveEngine::getfilesTransferUI(void) const
{
    return filesTransferUI.data();
}

FoldersManager* DriveEngine::getFoldersMngr(void) const
{
    return foldersMngr.data();
}

FoldersUI* DriveEngine::getFoldersUI(void) const
{
    return foldersUI.data();
}

QWidget* DriveEngine::getParent(void) const
{
    return parent;
}

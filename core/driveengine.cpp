#include "driveengine.h"
#include "share/registration.h"
#include "share/debug.h"
#include <QMessageBox>
#include "settings/settingsmanager.h"
#include <QHBoxLayout>
#include <QSplitter>

DriveEngine::DriveEngine(QObject *parent) :
    QObject(parent)
{
    this->parent = static_cast<QWidget*>(parent);
}

DriveEngine::~DriveEngine()
{
}

void DriveEngine::init(void)
{
    reset();
    setKeyActions();
    loadPanel(LEFT_PANEL, true);
}

void DriveEngine::reset(void)
{
    checkUI.reset(new CheckUI);

    for(int i = 0; i < EPanelsCount; ++i)
    {
        filesMngr[i].reset(new FilesManager);
        filesMngr[i]->init();
    }

    QHBoxLayout *hBoxLayout = new QHBoxLayout(SUi::inst()->panelsWidget);
    QSplitter *hSplitter = new QSplitter(Qt::Horizontal, SUi::inst()->panelsWidget);

    filesViewLeft = new FilePanel;
    filesViewRight = new FilePanel;

    hBoxLayout->setContentsMargins(0, 0, 0, 0);
    hBoxLayout->addWidget(hSplitter);

    hSplitter->setHandleWidth(1);

    hSplitter->addWidget(filesViewLeft);
    hSplitter->addWidget(filesViewRight);

    filesTransferUI.reset(new FilesTransferUI);
    filesUI.reset(new FilesUI);
    foldersMngr.reset(new FoldersManager);
    foldersUI.reset(new FoldersUI);
    oAuth2.reset(new OAuth2(parent));
    opUI.reset(new OperationsUI);
    opEventHandler.reset(new EventHandler<OperationsUI>(opUI.data()));
}

FilePanel* DriveEngine::getFilePanel(EPanels panel) const
{
    FilePanel *filePanel;

    if(panel == ELeft)
    {
        filePanel = filesViewLeft;
    }

    if(panel == ERight)
    {
        filePanel = filesViewRight;
    }

    return filePanel;
}

FilesManager* DriveEngine::getFilesMngr(bool opposite) const
{
    SettingsManager settingsManager;
    FilesManager* filesManager;

    if(settingsManager.currentPanel() == LEFT_PANEL)
    {
        if(opposite)
        {
            filesManager = filesMngr[ERight].data();
        }
        else
        {
            filesManager = filesMngr[ELeft].data();
        }
    }

    if(settingsManager.currentPanel() == RIGHT_PANEL)
    {
        if(opposite)
        {
            filesManager = filesMngr[ELeft].data();
        }
        else
        {
            filesManager = filesMngr[ERight].data();
        }
    }

    return filesManager;
}

void DriveEngine::loadPanel(const QString &panelName, bool initLoad)
{
    SettingsManager settingsManager;
    EPanels panel;
    QTreeWidget *treeWidget;
    QString query;

    settingsManager.setInitialLoading(initLoad);
    settingsManager.setCurrentPanel(panelName);

    if (panelName == RIGHT_PANEL)
    {
        panel  = ERight;
        treeWidget = filesViewRight->getFileView();
    }

    if (panelName == LEFT_PANEL)
    {
        panel = ELeft;
        treeWidget = filesViewLeft->getFileView();
    }

    int panelNum = static_cast <int> (panel);

    filesUI->getPanelLabel(panel)->setText(settingsManager.currentFolderPath(panelNum));
    query = settingsManager.currentFolderURL(panelNum);
    getFilesMngr()->setPathesURLs(settingsManager.pathesURLs(panelNum));

    filesMngr[panel]->setPanel(treeWidget);
    filesMngr[panel]->get(query);
}

void DriveEngine::slotFirstPanelIsLoaded()
{
    loadPanel(RIGHT_PANEL, false);
}

void DriveEngine::setKeyActions(void)
{
    opEventHandler->setKeyAction(Qt::Key_Delete, &OperationsUI::del);
}

void DriveEngine::slotStartLogin()
{
    oAuth2->startLogin(false);
}

void DriveEngine::slotStartLoginFromMenu()
{
    oAuth2->startLogin(true);
}

CheckUI* DriveEngine::getCheckUI(void) const
{
    return checkUI.data();
}

EventHandler<OperationsUI>* DriveEngine::getEventHandler(void) const
{
    return opEventHandler.data();
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

OAuth2* DriveEngine::getOAuth2(void) const
{
    return oAuth2.data();
}

OperationsUI* DriveEngine::getOpUI(void) const
{
    return opUI.data();
}

QWidget* DriveEngine::getParent(void) const
{
    return parent;
}

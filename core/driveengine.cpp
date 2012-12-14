#include "driveengine.h"
#include "share/registration.h"
#include "share/debug.h"
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QHBoxLayout>

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
    loadPanel(RIGHT_PANEL, true);
}

void DriveEngine::reset(void)
{
    DEBUG << "RESET";

    checkUI.reset(new CheckUI);

    for(int i = 0; i < EPanelsCount; ++i)
    {
        filesMngr[i].reset(new FilesManager);
        filesMngr[i]->init();
    }

    QHBoxLayout *hBoxLayout = new QHBoxLayout(SUi::inst()->panelsWidget);

    filesViewLeft = new FilePanel;
    filesViewRight = new FilePanel;

    hBoxLayout->setContentsMargins(0, 0, 0, 0);
    hBoxLayout->setSpacing(1);

    hBoxLayout->addWidget(filesViewLeft);
    hBoxLayout->addWidget(filesViewRight);

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
    QSettings settings(COMPANY_NAME, APP_NAME);
    FilesManager* filesManager;

    if(settings.value(CURRENT_PANEL, LEFT_PANEL).toString() == LEFT_PANEL)
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

    if(settings.value(CURRENT_PANEL, RIGHT_PANEL).toString() == RIGHT_PANEL)
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
    QSettings settings(COMPANY_NAME, APP_NAME);
    EPanels panel;
    QTreeWidget *treeWidget;
    QString query;

    settings.setValue(INIT_LOAD, initLoad);
    settings.setValue(CURRENT_PANEL, panelName);

    if (panelName == RIGHT_PANEL)
    {
        panel  = ERight;
        //treeWidget = SUi::inst()->filesViewRight;
        treeWidget = filesViewRight->getFileView();
    }

    if (panelName == LEFT_PANEL)
    {
        panel = ELeft;
        //treeWidget = SUi::inst()->filesViewLeft;
        treeWidget = filesViewLeft->getFileView();
    }

    settings.beginGroup(PANEL + QString::number(static_cast <int> (panel)));

    filesUI->getPanelLabel(panel)->setText(settings.value(CURRENT_FOLDER_PATH, QString("a:") + QDir::toNativeSeparators("/")).toString());
    query = settings.value(CURRENT_FOLDER_URL, GET_FULL_ROOT_CONTENT).toString();
    SDriveEngine::inst()->getFilesMngr()->setPathesURLs(settings.value(PATHES_URLS).toStringList());

    settings.endGroup();

    SDriveEngine::inst()->filesMngr[panel]->setPanel(treeWidget);
    SDriveEngine::inst()->filesMngr[panel]->get(query);
}

void DriveEngine::slotFirstPanelIsLoaded()
{
    loadPanel(LEFT_PANEL, false);
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

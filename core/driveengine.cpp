#include "driveengine.h"
#include "share/appregdata.h"
#include <QMessageBox>
#include <QSettings>
#include <QDebug>


DriveEngine::DriveEngine(QObject *parent) :
    QObject(parent)
{
    this->parent = static_cast<QWidget*>(parent);
    for(int i = 0; i < EElStatesCount; ++i) elStates[i] = false;
}

DriveEngine::~DriveEngine()
{
}

void DriveEngine::init(void)
{
    reset();
    setKeyActions();

    QSettings settings(COMPANY_NAME, APP_NAME);
    settings.setValue(INIT_LOAD, true);

    //foldersUI->showFolders(GET_FOLDERS_TREE);

    settings.setValue(CURRENT_PANEL, RIGHT_PANEL);
    SDriveEngine::inst()->filesMngr[ERight]->setPanel(SUi::inst()->filesViewRight);
    SDriveEngine::inst()->filesMngr[ERight]->get(GET_FULL_ROOT_CONTENT);

    filesUI->setDisc(ERight);
    filesUI->setDisc(ELeft);

    //foldersUI->createAFolders();
}

void DriveEngine::reset(void)
{
    //aFoldersMngr.reset(new AdditionalFoldersManager);
    checkUI.reset(new CheckUI);
    for(int i = 0; i < EPanelsCount; ++i) filesMngr[i].reset(new FilesManager);
    filesTransferUI.reset(new FilesTransferUI);
    filesUI.reset(new FilesUI);
    foldersMngr.reset(new FoldersManager);
    foldersUI.reset(new FoldersUI);
    oAuth2.reset(new OAuth2(parent));
    opUI.reset(new OperationsUI);
    opEventHandler.reset(new EventHandler<OperationsUI>(opUI.data()));
}

void DriveEngine::setKeyActions(void)
{
    opEventHandler->setKeyAction(Qt::Key_Delete, &OperationsUI::del);
}

void DriveEngine::slotStartLogin()
{
    qDebug() << "DriveEngine::slotStartLogin";

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

FilesManager* DriveEngine::getCurrFilesMngr(void) const
{
    QSettings settings(COMPANY_NAME, APP_NAME);
    FilesManager* fm;

    if(settings.value(CURRENT_PANEL, LEFT_PANEL).toString() == LEFT_PANEL)
    {
        fm = filesMngr[ELeft].data();
    }
    if(settings.value(CURRENT_PANEL, RIGHT_PANEL).toString() == RIGHT_PANEL)
    {
        fm = filesMngr[ERight].data();
    }

    return fm;
}

void DriveEngine::slotFirstPanelIsLoaded()
{
    QSettings settings(COMPANY_NAME, APP_NAME);

    settings.setValue(CURRENT_PANEL, LEFT_PANEL);
    settings.setValue(INIT_LOAD, false);

    SDriveEngine::inst()->filesMngr[ELeft]->setPanel(SUi::inst()->filesViewLeft);
    SDriveEngine::inst()->filesMngr[ELeft]->get(GET_FULL_ROOT_CONTENT);
}

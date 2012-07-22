#include "driveengine.h"
#include <QDebug>
#include "AppRegData.h"
#include <QMessageBox>

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

    foldersUI->showFolders(GET_FOLDERS_TREE);
    SDriveEngine::inst()->filesMngr->get(GET_FULL_ROOT_CONTENT);
    foldersUI->createAFolders();
}

void DriveEngine::reset(void)
{
    aFoldersMngr.reset(new AdditionalFoldersManager);
    checkUI.reset(new CheckUI);
    filesMngr.reset(new FilesManager);
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

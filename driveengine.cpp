#include "driveengine.h"
#include <QDebug>
#include "AppRegData.h"
#include <QMessageBox>

DriveEngine::DriveEngine(QObject *parent) :
    QObject(parent)
{
    this->parent = static_cast<QWidget*>(parent);
    for(int i = 0; i < EElementsStatesCount; ++i) elementsStates[i] = false;
}

DriveEngine::~DriveEngine()
{
}

void DriveEngine::init(void)
{
    reset();
    setKeyActions();

    foldersUI->showFolders();
    foldersUI->showAdditionalFolders();
}

void DriveEngine::reset(void)
{
    additionalFilesManager.reset(new AdditionalFoldersManager);
    checkUI.reset(new CheckUI);   
    filesManager.reset(new FilesManager);
    filesTransfer.reset(new FilesTransferUI);
    filesUI.reset(new FilesUI);
    foldersManager.reset(new FoldersManager);
    foldersUI.reset(new FoldersUI);
    oAuth2.reset(new OAuth2(parent));
    operationsUI.reset(new OperationsUI);
    opEventHandler.reset(new EventHandler<OperationsUI>(operationsUI.data()));
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
    return filesTransfer.data();
}

FoldersManager* DriveEngine::getFoldersManager(void) const
{
    return foldersManager.data();
}

FoldersUI* DriveEngine::getFoldersUI(void) const
{
    return foldersUI.data();
}

OAuth2* DriveEngine::getOAuth2(void) const
{
    return oAuth2.data();
}

OperationsUI* DriveEngine::getOperationsUI(void) const
{
    return operationsUI.data();
}

QWidget* DriveEngine::getParent(void) const
{
    return parent;
}

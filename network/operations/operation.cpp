#include "operation.h"
#include "core/driveengine.h"
#include "network/webcontentmanager.h"
#include "settings/settingsmanager.h"

Operation::Operation(EOperations oId) :
    operationId(oId)
{
}

void Operation::updatePanelContent(bool opposite)
{
    SettingsManager settingsManager;
    QString currentFolderURL;

    if(opposite) currentFolderURL = settingsManager.currentFolderUrl(settingsManager.oppositePanel());
    else currentFolderURL = settingsManager.currentFolderUrl(settingsManager.currentPanel());

    ContentManager* cm = SDriveEngine::inst()->getContentMngr(opposite);
    //cm->getPanel()->clear();
    cm->get(currentFolderURL);

    if(SDriveEngine::inst()->isPanelsContentIdentical())
    {
        cm = SDriveEngine::inst()->getContentMngr(!opposite);
        //cm->getPanel()->clear();
        cm->get(currentFolderURL);
    }
}

EOperations Operation::getOperationId() const
{
  return operationId;
}

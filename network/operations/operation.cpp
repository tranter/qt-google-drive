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
    int oppositePanel = settingsManager.oppositePanel();
    int currentPanel = settingsManager.currentPanel();
    int currentPanelState = settingsManager.panelState(settingsManager.currentPanel());
    int oppositePanelState = settingsManager.panelState(oppositePanel);

    if(!opposite)
    {
        if(currentPanelState == EWeb)
        {
            currentFolderURL = settingsManager.currentFolderUrl(currentPanel);
        }
    }
    else
    {
        if(oppositePanelState == EWeb)
        {
            currentFolderURL = settingsManager.currentFolderUrl(oppositePanel);
        }

        if(oppositePanelState == EComputer)
        {
            ContentManager* cm = SDriveEngine::inst()->getContentMngr(opposite);
            cm->update();
        }
    }

    if(currentPanelState == EWeb && oppositePanelState == EWeb)
    {
        ContentManager* cm = SDriveEngine::inst()->getContentMngr(opposite);
        cm->get(currentFolderURL);

        if(SDriveEngine::inst()->isPanelsContentIdentical())
        {
            cm = SDriveEngine::inst()->getContentMngr(!opposite);
            cm->get(currentFolderURL);
        }
    }
}

EOperations Operation::getOperationId() const
{
    return operationId;
}

#include "operation.h"
#include "core/driveengine.h"
#include "network/webcontentmanager.h"
#include "settings/settingsmanager.h"

Operation::Operation()
{
}

void Operation::updatePanelContent(bool opposite)
{
    SettingsManager settingsManager;
    QString currentFolderURL(settingsManager.currentFolderUrl(settingsManager.currentPanel()));

    ContentManager* cm = SDriveEngine::inst()->getContentMngr(opposite);
    cm->get(currentFolderURL);

    if(SDriveEngine::inst()->isPanelsContentIdentical())
    {
      cm = SDriveEngine::inst()->getContentMngr(!opposite);
      cm->get(currentFolderURL);
    }
}

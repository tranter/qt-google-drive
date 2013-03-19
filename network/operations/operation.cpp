#include "operation.h"
#include "core/driveengine.h"
#include "network/webcontentmanager.h"

Operation::Operation()
{
}

void Operation::updatePanelContent(bool opposite)
{
    WebContentManager* cm = SDriveEngine::inst()->getWebContentMngr(opposite);
    cm->get(cm->getParentFolder());

    if(SDriveEngine::inst()->isPanelsContentIdentical())
    {
      cm = SDriveEngine::inst()->getWebContentMngr(!opposite);
      cm->get(cm->getParentFolder());
    }
}

#include "operation.h"
#include "core/driveengine.h"
#include "network/contentmanager.h"

Operation::Operation()
{
}

void Operation::updatePanelContent(bool opposite)
{
    ContentManager* cm = SDriveEngine::inst()->getContentMngr(opposite);
    cm->get(cm->getParentFolderUrl());

    DEBUG << "!!!!!!!!!!";

    if(SDriveEngine::inst()->isPanelsContentIdentical())
    {
      DEBUG << "???????????????";
      cm = SDriveEngine::inst()->getContentMngr(!opposite);
      cm->get(cm->getParentFolderUrl());
    }
}

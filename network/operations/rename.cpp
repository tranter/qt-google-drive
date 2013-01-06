#include "rename.h"
#include "settings/settingsmanager.h"
#include "network/operations/operationsmanager.h"

Rename::Rename(QObject *parent) :
    NetworkManager(parent)
{
}

void Rename::item(const Items::Data &source, const QString &newName)
{
    postData = queries.getRenameFileData(newName);
    queries.setRawHeader(SettingsManager().accessToken(), request);
    putRequest(QUrl(queries.constructRenameWebFileUrl(source.self)));
}

void Rename::slotPutFinished(void)
{
    SOperationsManager::inst()->updatePanelContent(false);
}

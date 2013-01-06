#include "rename.h"
#include "settings/settingsmanager.h"
#include "network/operations/operationsmanager.h"
#include "share/debug.h"

Rename::Rename(QObject *parent) :
    NetworkManager(parent)
{
}

void Rename::file(const Items::Data &source, const QString &newName)
{
    DEBUG;
    postData = queries.getRenameFileData(newName);
    queries.setRawHeader(SettingsManager().accessToken(), request);
    putRequest(QUrl(queries.constructRenameWebFileUrl(source.self)));
}

void Rename::slotPutFinished(void)
{
    DEBUG;
    SOperationsManager::inst()->updatePanelContent(false);
}

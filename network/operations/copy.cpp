#include "copy.h"
#include "settings/settingsmanager.h"
#include "network/operations/operationsmanager.h"

Copy::Copy(QObject *parent) :
    NetworkManager(parent)
{
}

void Copy::file(const Items::Data &source, const QString &destFolderUrl)
{
    postData = queries.getCopyWebFileData(source.name, destFolderUrl);
    queries.setRawHeader(SettingsManager().accessToken(), request);
    postRequest(queries.constructCopyWebFileUrl(source.self));
}

void Copy::slotPostFinished(QNetworkReply* reply)
{
    NetworkManager::slotPostFinished(reply);
    SOperationsManager::inst()->updatePanelContent(true);
}

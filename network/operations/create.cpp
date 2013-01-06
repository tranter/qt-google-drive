#include "create.h"
#include "settings/settingsmanager.h"
#include "network/operations/operationsmanager.h"

Create::Create(QObject *parent) :
    NetworkManager(parent)
{
}

void Create::folder(const QString &name, const QString &parentFolderUrl)
{    
    postData = queries.getCreateFolderData(name, parentFolderUrl);

    queries.setRawHeader(SettingsManager().accessToken(), request);
    postRequest(queries.constructCreateFolderUrl());
}

void Create::slotPostFinished(QNetworkReply* reply)
{
    NetworkManager::slotPostFinished(reply);
    SOperationsManager::inst()->updatePanelContent(false);
}

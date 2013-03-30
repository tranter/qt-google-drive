#include "copy.h"
#include "settings/settingsmanager.h"

Copy::Copy(QObject *parent) :
    NetworkManager(parent),
    Operation(ECopy)
{
}

void Copy::file(const Items::Data &source, const QString &destFolderUrl)
{
    sourceData = source;
    postData = queries.getCopyFileData(source.name, destFolderUrl);

    queries.setRawHeader(SettingsManager().accessToken(), request);
    postRequest(queries.constructCopyFileUrl(source.self));
}

void Copy::files(const QList<Items::Data> &sources, const QString &destFolderUrl)
{
    sourcesData = sources;
    destFolderUrlData = destFolderUrl;

    if(!sources.isEmpty()) file(sourcesData.takeFirst(), destFolderUrl);
}

void Copy::slotPostFinished(QNetworkReply *reply)
{
 //   NetworkManager::slotPostFinished(reply);

    if(sourcesData.isEmpty())
    {
        emit fileCopied(sourceData, true);
        updatePanelContent(true);
    }
    else
    {
        emit fileCopied(sourceData);
        file(sourcesData.takeFirst(), destFolderUrlData);
    }
}

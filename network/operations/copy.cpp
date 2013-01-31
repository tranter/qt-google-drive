#include "copy.h"
#include "settings/settingsmanager.h"

Copy::Copy(QObject *parent) :
    NetworkManager(parent)
{
}

void Copy::file(const Items::Data &source, const QString &destFolderUrl)
{
    DEBUG << source.name;

    sourceData = source;
    postData = queries.getCopyFileData(source.name, destFolderUrl);

    queries.setRawHeader(SettingsManager().accessToken(), request);
    postRequest(queries.constructCopyFileUrl(source.self));
}

void Copy::files(const QList<Items::Data> &sources, const QString &destFolderUrl)
{
    sourcesData = sources;
    destFolderUrlData = destFolderUrl;

    connect(this, SIGNAL(fileCopied(Items::Data&)), this, SLOT(slotFileCopied(Items::Data&)));

    if(!sources.isEmpty())
    {
        file(sourcesData.takeFirst(), destFolderUrl);
    }
}

void Copy::slotPostFinished(QNetworkReply *reply)
{
    NetworkManager::slotPostFinished(reply);
    updatePanelContent(true);

    emit fileCopied(sourceData);
}

void Copy::slotFileCopied(Items::Data& itemData)
{
    if(sourcesData.isEmpty())
    {
        disconnect(this, SIGNAL(fileCopied(Items::Data&)), this, SLOT(slotFileCopied(Items::Data&)));
    }
    else
    {
        file(sourcesData.takeFirst(), destFolderUrlData);
    }
}

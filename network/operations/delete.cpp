#include "delete.h"
#include "settings/settingsmanager.h"

Delete::Delete(QObject *parent):
    NetworkManager(parent),
    Operation(EDelete)
{
}

void Delete::item(const Items::Data &source, bool update)
{
    sourceData = source;
    updateItemAfterDelete = update;

    queries.setRawHeader(SettingsManager().accessToken(), request);
    deleteRequest(queries.constructDeleteFileUrl(source.self));
}

void Delete::items(const QList<Items::Data> &sources)
{
    sourcesData = sources;
    if(!sources.isEmpty()) item(sourcesData.takeFirst(), false);
}

void Delete::slotReplyFinished(QNetworkReply*)
{
    emit itemDeleted(sourceData);

    if(sourcesData.isEmpty() && updateItemAfterDelete) updatePanelContent(false);

    if(!sourcesData.isEmpty())
    {
        Items::Data source = sourcesData.takeFirst();

        if(sourcesData.isEmpty()) item(source, true);
        else item(source, false);
    }
}

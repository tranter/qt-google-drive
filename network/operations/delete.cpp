#include "delete.h"
#include "settings/settingsmanager.h"

Delete::Delete(QObject *parent)
    :NetworkManager(parent)
{
}

void Delete::item(const Items::Data &source)
{
    queries.setRawHeader(SettingsManager().accessToken(), request);
    deleteRequest(queries.constructDeleteWebFileUrl(source.self));
}

void Delete::slotReplyFinished(QNetworkReply*)
{
   updatePanelContent(false);
}

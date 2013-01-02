#include "queries.h"
#include "share/commontools.h"
#include "settings/settingsmanager.h"
#include <QString>

Queries::Queries()
{
}

void Queries::setRawHeader(const QString &accessToken, QNetworkRequest &request)
{
    request.setRawHeader("User-Agent", SettingsManager().applicationName().toAscii());
    //request.setRawHeader("GData-Version", "3.0");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(accessToken).toAscii());
    request.setRawHeader("Content-Type", "application/json");
}

void Queries::userAboutInfo(QString &userInfoQuery, QString &aboutInfoQuery)
{
    userInfoQuery = QString("https://www.googleapis.com/oauth2/v1/userinfo");
    aboutInfoQuery = QString("https://www.googleapis.com/drive/v2/about");
}

QByteArray Queries::getCopyWebFileData(const QString &sourceName, const QString &destFolderUrl)
{
    return QString("{\"kind\": \"drive#file\", \"title\": \"%1\",\"parents\": [{\"id\":\"%2\"}]}").arg(sourceName).arg(CommonTools::getIDFromURL(destFolderUrl)).toAscii();
}

QUrl Queries::constructCopyWebFileUrl(const QString &url)
{
    return QUrl(QString(QString("https://www.googleapis.com/drive/v2/files/") + CommonTools::getIDFromURL(url) + QString("/copy")));
}


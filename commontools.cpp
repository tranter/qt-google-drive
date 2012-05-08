#include "commontools.h"

void CommonTools::setHeader(QNetworkRequest& request)
{
    QSettings settings(COMPANY_NAME, APP_NAME);
    QString accessToken = settings.value("access_token").toString();

    request.setRawHeader("User-Agent", APP_NAME);
    request.setRawHeader("GData-Version", "3.0");
    request.setRawHeader("Authorization",(QString("OAuth %1").arg(accessToken)).toLatin1());
}

#include "queries.h"
#include "share/defs.h"
#include "share/registration.h"
#include "share/debug.h"
#include <QSettings>

Queries::Queries()
{
}

void Queries::getAccountInfo(void)
{
    QSettings settings(COMPANY_NAME, APP_NAME);
    QString accessToken = settings.value(ACCESS_TOKEN).toString();

    QString userInfoQuery(QString("https://www.googleapis.com/oauth2/v1/userinfo?access_token=").arg(accessToken));
    QString aboutInfoQuery(QString("https://www.googleapis.com/drive/v2/about"));

    userInfo.reset(new AccountInfo(userInfoQuery, aboutInfoQuery));

    userInfo->getInfo();
}

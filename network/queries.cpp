#include "queries.h"
#include "share/defs.h"
#include "share/registration.h"
#include "share/debug.h"
#include <QSettings>

Queries::Queries()
{
}

void Queries::setAccountInfo(void)
{
    QSettings settings(COMPANY_NAME, APP_NAME);

    QString userInfoQuery(QString("https://www.googleapis.com/oauth2/v1/userinfo?access_token=").arg(settings.value(ACCESS_TOKEN).toString()));
    QString aboutInfoQuery(QString("https://www.googleapis.com/drive/v2/about"));

    accountInfo.reset(new AccountInfo(userInfoQuery, aboutInfoQuery));

    accountInfo->setInfo();
}

AccountInfo *Queries::accountInfoPointer(void) const
{
   return accountInfo.data();
}

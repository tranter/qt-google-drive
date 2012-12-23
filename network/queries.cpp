#include "queries.h"
#include "share/defs.h"
#include "share/registration.h"
#include "share/debug.h"
#include "settings/settingsmanager.h"

Queries::Queries()
{
}

void Queries::setAccountInfo(void)
{
    DEBUG;

    //QString userInfoQuery(QString("https://www.googleapis.com/oauth2/v1/userinfo?access_token=%1").arg(SettingsManager().accessToken()));
    QString userInfoQuery(QString("https://www.googleapis.com/oauth2/v1/userinfo"));
    QString aboutInfoQuery(QString("https://www.googleapis.com/drive/v2/about"));

//    QString userInfoQuery(QString("https://www.googleapis.com/drive/v2/about"));
//    QString aboutInfoQuery(QString("https://www.googleapis.com/oauth2/v1/userinfo"));

    accountInfo = new AccountInfo(userInfoQuery, aboutInfoQuery);

    connect(accountInfo, SIGNAL(signalAccountInfo(AccountInfo::Data&)), this, SLOT(slotAccountInfo(AccountInfo::Data&)));

    accountInfo->setInfo();
}

void Queries::slotAccountInfo(AccountInfo::Data &data)
{
    DEBUG;

    SettingsManager().writeAccountInfo(data);

    accountInfo->deleteLater();
    emit signalAccountInfoReadyToUse();
}

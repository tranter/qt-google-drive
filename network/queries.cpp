#include "queries.h"
#include "share/defs.h"
#include "share/registration.h"
#include "settings/settingsmanager.h"
#include "share/enums.h"
#include "share/debug.h"

Queries::Queries()
{
}

void Queries::setAccountInfo(const QString &accessToken, const QString &refreshToken)
{
    DEBUG << "accessToken" << accessToken << " refreshToken" << refreshToken;

    QString userInfoQuery(QString("https://www.googleapis.com/oauth2/v1/userinfo"));
    QString aboutInfoQuery(QString("https://www.googleapis.com/drive/v2/about"));

    accountInfo = new AccountInfo(userInfoQuery, aboutInfoQuery, accessToken, refreshToken);

    connect(accountInfo, SIGNAL(signalAccountInfo(AccountInfo::Data&)), this, SLOT(slotAccountInfo(AccountInfo::Data&)));

    accountInfo->setInfo();
}

void Queries::slotAccountInfo(AccountInfo::Data &data)
{
    DEBUG << data.email;

    SettingsManager settingsManager;

    if(!settingsManager.isAnyAccount())
    {
        DEBUG << "!settingsManager.isAnyAccount";
        settingsManager.setCurrentAccount(static_cast<int> (ELeft), data.email);
        settingsManager.setCurrentAccount(static_cast<int> (ERight), data.email);
    }

    settingsManager.writeAccountInfo(data);

    accountInfo->deleteLater();
    emit signalAccountInfoReadyToUse();
}

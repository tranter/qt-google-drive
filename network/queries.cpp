#include "queries.h"

Queries::Queries()
{
}

//void Queries::setAccountInfo(const QString &accessToken, const QString &refreshToken)
//{
//    QString userInfoQuery(QString("https://www.googleapis.com/oauth2/v1/userinfo"));
//    QString aboutInfoQuery(QString("https://www.googleapis.com/drive/v2/about"));

//    accountInfo = new AccountInfo(userInfoQuery, aboutInfoQuery, accessToken, refreshToken);

//    connect(accountInfo, SIGNAL(signalAccountInfo(AccountInfo::Data&)), this, SLOT(slotAccountInfo(AccountInfo::Data&)));

//    accountInfo->setInfo();
//}

//void Queries::slotAccountInfo(AccountInfo::Data &data)
//{
//    SettingsManager settingsManager;

//    if(!settingsManager.isAnyAccount())
//    {
//        settingsManager.setCurrentAccount(static_cast<int> (ELeft), data.email);
//        settingsManager.setCurrentAccount(static_cast<int> (ERight), data.email);
//    }

//    settingsManager.saveAccountInfo(data);

//    accountInfo->deleteLater();
//    emit signalAccountInfoReadyToUse();
//}

void Queries::userAboutInfo(QString &userInfoQuery, QString &aboutInfoQuery)
{
    userInfoQuery = QString("https://www.googleapis.com/oauth2/v1/userinfo");
    aboutInfoQuery = QString("https://www.googleapis.com/drive/v2/about");
}

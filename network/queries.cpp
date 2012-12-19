#include "queries.h"
#include "share/defs.h"
#include "share/registration.h"
#include "share/debug.h"

Queries::Queries()
{
}

void Queries::setAccountInfo(const QString &accessToken)
{
    QString userInfoQuery(QString("https://www.googleapis.com/oauth2/v1/userinfo?access_token=").arg(accessToken));
    QString aboutInfoQuery(QString("https://www.googleapis.com/drive/v2/about"));

    accountInfo = new AccountInfo(userInfoQuery, aboutInfoQuery);

    connect(accountInfo, SIGNAL(signalAccountInfo(AccountInfo::Data&)), this, SLOT(slotAccountInfo(AccountInfo::Data&)));

    accountInfo->setInfo();
}

void Queries::slotAccountInfo(AccountInfo::Data &data)
{
    DEBUG << "=========================> data.name" << data.name;
    DEBUG << "=========================> data.email" << data.email;
    DEBUG << "=========================> data.domainSharingPolicy" << data.domainSharingPolicy;
    DEBUG << "=========================> data.permissionId" <<  data.permissionId;
    DEBUG << "=========================> data.quotaBytesTotal" << QString::number(data.quotaBytesTotal);
    DEBUG << "=========================> data.quotaBytesUsed" << QString::number(data.quotaBytesUsed);
    DEBUG << "=========================> data.quotaBytesUsedInTrash" << QString::number(data.quotaBytesUsedInTrash);

    delete accountInfo;

    emit signalAccountInfoReadyToUse();
}

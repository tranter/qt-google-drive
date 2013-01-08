#include "accountinfo.h"
#include "share/commontools.h"
#include "parsers/jsonparser.h"
#include "network/queries.h"
#include "share/debug.h"

AccountInfo::AccountInfo(const QString &uiq, const QString &aiq, const QString &at, const QString &rt) :
    userInfoQuery(uiq),
    aboutInfoQuery(aiq),
    query(EUserInfoQuery),
    queryStr(userInfoQuery)
{
    accountData.accessToken = at;
    accountData.refreshToken = rt;
}

void AccountInfo::slotReplyFinished(QNetworkReply*)
{
    parseReply();

    replyStr.clear();

    if(query == EUserInfoQuery)
    {
        queryStr = aboutInfoQuery;
        query = EAboutInfoQuery;

        setInfo();
    }
    else if(query == EAboutInfoQuery)
    {
        queryStr = userInfoQuery;
        query = EUserInfoQuery;

        emit signalAccountInfoReceived(accountData);
    }
}

void AccountInfo::setInfo(void)
{
    Queries().setRawHeader(accountData.accessToken, request);
    getRequest(queryStr);
}

void AccountInfo::parseReply(void)
{
    DEBUG << "<===============================================================================================================";
    DEBUG << "replyStr" << replyStr;
    DEBUG << "===============================================================================================================>";

    JSONParser jParser;

    if(query == EUserInfoQuery)
    {
        accountData.name = jParser.getParam(replyStr, QString("name"));
        accountData.email = jParser.getParam(replyStr, QString("email"));
    }
    else if(query == EAboutInfoQuery)
    {
        accountData.domainSharingPolicy = jParser.getParam(replyStr, QString("domainSharingPolicy"));
        accountData.permissionId = jParser.getParam(replyStr, QString("permissionId"));
        accountData.quotaBytesTotal = jParser.getParam(replyStr, QString("quotaBytesTotal")).toLongLong();
        accountData.quotaBytesUsed = jParser.getParam(replyStr, QString("quotaBytesUsed")).toLongLong();
        accountData.quotaBytesUsedInTrash = jParser.getParam(replyStr, QString("quotaBytesUsedInTrash")).toLongLong();
    }
}



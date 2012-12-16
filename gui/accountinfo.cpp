#include "accountinfo.h"
#include "share/commontools.h"
#include "parsers/jsonparser.h"
#include "share/debug.h"

AccountInfo::AccountInfo(const QString &uiq, const QString &aiq) :
    userInfoQuery(uiq),
    aboutInfoQuery(aiq),
    query(EUserInfoQuery),
    queryStr(userInfoQuery)
{
}

void AccountInfo::slotReplyFinished(QNetworkReply*)
{
    if(parseReply(replyStr))
    {
        if(query == EUserInfoQuery)
        {
            queryStr = aboutInfoQuery;
            query = EAboutInfoQuery;

            getInfo();
        }
        else if(query == EAboutInfoQuery)
        {
            queryStr = userInfoQuery;
            query = EUserInfoQuery;
        }
    }

    replyStr.clear();
}

void AccountInfo::getInfo(void)
{
    CommonTools::setHeader(request);
    request.setRawHeader("Content-Type", "application/json");

    getRequest(queryStr);
}

bool AccountInfo::parseReply(const QString &str)
{
//    DEBUG << "<===============================================================================================================";
//    DEBUG << "replyStr" << replyStr;
//    DEBUG << "===============================================================================================================>";

    JSONParser jParser;

    if(query == EUserInfoQuery)
    {
        accountData.name = jParser.getParam(replyStr, QString("email"));
        accountData.email = jParser.getParam(replyStr, QString("name"));
    }
    else if(query == EAboutInfoQuery)
    {
        accountData.domainSharingPolicy = jParser.getParam(replyStr, QString("domainSharingPolicy"));
        accountData.permissionId = jParser.getParam(replyStr, QString("permissionId"));
        accountData.quotaBytesTotal = jParser.getParam(replyStr, QString("quotaBytesTotal")).toLong();
        accountData.quotaBytesUsed = jParser.getParam(replyStr, QString("quotaBytesUsed")).toLong();
        accountData.quotaBytesUsedInTrash = jParser.getParam(replyStr, QString("quotaBytesUsedInTrash")).toLong();
    }

    return true;
}



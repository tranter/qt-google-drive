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
    parseReply();

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
    }


    replyStr.clear();
}

void AccountInfo::setInfo(void)
{
    CommonTools::setHeader(request);
    request.setRawHeader("Content-Type", "application/json");

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
        accountData.name = jParser.getPlainParam(replyStr, QString("name"));
        accountData.email = jParser.getPlainParam(replyStr, QString("email"));
    }
    else if(query == EAboutInfoQuery)
    {
        accountData.domainSharingPolicy = jParser.getPlainParam(replyStr, QString("domainSharingPolicy"));
        accountData.permissionId = jParser.getPlainParam(replyStr, QString("permissionId"));
        accountData.quotaBytesTotal = jParser.getPlainParam(replyStr, QString("quotaBytesTotal")).toLongLong();
        accountData.quotaBytesUsed = jParser.getPlainParam(replyStr, QString("quotaBytesUsed")).toLongLong();
        accountData.quotaBytesUsedInTrash = jParser.getPlainParam(replyStr, QString("quotaBytesUsedInTrash")).toLongLong();
    }

    DEBUG << "-------------------------------> accountData.name" << accountData.name;
    DEBUG << "-------------------------------> accountData.email" << accountData.email;
    DEBUG << "-------------------------------> accountData.domainSharingPolicy" << accountData.domainSharingPolicy;
    DEBUG << "-------------------------------> accountData.permissionId" <<  accountData.permissionId;
    DEBUG << "-------------------------------> accountData.quotaBytesTotal" << QString::number(accountData.quotaBytesTotal);
    DEBUG << "-------------------------------> accountData.quotaBytesUsed" << QString::number(accountData.quotaBytesUsed);
    DEBUG << "-------------------------------> accountData.quotaBytesUsedInTrash" << QString::number(accountData.quotaBytesUsedInTrash);

}

AccountInfo::Data AccountInfo::getData(void) const
{
    return accountData;
}



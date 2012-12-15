#include "accountinfo.h"
#include "share/commontools.h"
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
    DEBUG << "<===============================================================================================================";
    DEBUG << "replyStr" << replyStr;
    DEBUG << "===============================================================================================================>";

    return true;
}



#include "auth.h"
#include "parsers/jsonparser.h"
#include "share/debug.h"

Auth::Auth(QObject *parent) :
    NetworkManager(parent)
{
}

QUrl Auth::getOAuth2CodeUrl(const QString &scope, const QString &redirectUri, const QString &clientId, bool accessType, bool approvalPrompt, const QString &state)
{
    //    QString urlStr(QString("https://accounts.google.com/o/oauth2/auth?scope=%1&redirect_uri=%2&response_type=code&client_id=%3&access_type=%4&approval_prompt=%5&state=%6")
    //                   .arg(scope)
    //                   .arg(redirectUri)
    //                   .arg(clientId)
    //                   .arg(accessType ? QString("online") : QString("offline"))
    //                   .arg(approvalPrompt ? QString("force") : QString("auto"))
    //                   .arg(state));

    QUrl url("https://accounts.google.com/o/oauth2/auth?");
    QList<QPair<QString, QString> > query;

    query.append(qMakePair(QString("scope"), scope));
    query.append(qMakePair(QString("redirect_uri"), redirectUri));
    query.append(qMakePair(QString("response_type"), QString("code")));
    query.append(qMakePair(QString("client_id"), clientId));
    query.append(qMakePair(QString("access_type"), accessType ? QString("online") : QString("offline")));
    query.append(qMakePair(QString("approval_prompt"), approvalPrompt ? QString("force") : QString("auto")));
    query.append(qMakePair(QString("state"), state));

    url.setQueryItems(query);

    return url;
}

QString Auth::getOAuth2Code(const QUrl &url)
{
    return QString(url.queryItemValue("code"));
}

void Auth::getTokens(const QString &code, const QString &clientId, const QString &clientSecret, const QString &redirectUri)
{
    currentRequest = EAllTokens;

    postData = "code=";
    postData += code.toAscii();
    postData += "&client_id=";
    postData += clientId.toAscii();
    postData += "&client_secret=";
    postData += clientSecret.toAscii();
    postData += "&redirect_uri=";
    postData += redirectUri.toAscii();
    postData += "&grant_type=authorization_code";

    performRequest();
}

void Auth::getAccessToken(const QString &clientId, const QString &clientSecret, const QString &refreshToken)
{
    currentRequest = EAccessToken;

    postData = "client_id=";
    postData += clientId.toAscii();
    postData += "&client_secret=";
    postData += clientSecret.toAscii();
    postData += "&refresh_token=";
    postData += refreshToken.toAscii();
    postData += "&grant_type=refresh_token";

    performRequest();
}

void Auth::postFinishedActions(QNetworkReply* reply)
{
    replyStr = reply->readAll();

    JSONParser jParser;

    QString accessToken(jParser.getPlainParam(replyStr, "access_token"));
    QString refreshToken(jParser.getPlainParam(replyStr, "refresh_token"));

    if(currentRequest == EAllTokens)
    {
        DEBUG << "EAllTokens";
        emit authResponse(accessToken, refreshToken);
    }

    if(currentRequest == EAccessToken)
    {
        DEBUG << "EAccessToken";
        emit authResponse(accessToken);
    }
}

void Auth::performRequest(void)
{
    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");
    postRequest(QUrl("https://accounts.google.com/o/oauth2/token"));
}

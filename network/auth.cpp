#include "auth.h"
#include "parsers/jsonparser.h"
#include "share/debug.h"
#include "share/registration.h"

#if QT_VERSION >= 0x050000
#include <QUrlQuery>
#endif


Auth::Auth(QObject *parent) :
    NetworkManager(parent)
{
}

Auth::Auth::~Auth()
{
}

QUrl Auth::getOAuth2CodeUrl(const QString &scope, const QString &redirectUri, const QString &clientId, bool accessType, bool approvalPrompt, const QString &state)
{
    QUrl url("https://accounts.google.com/o/oauth2/auth?");
    QList<QPair<QString, QString> > query;

    query.append(qMakePair(QString("scope"), scope));
    query.append(qMakePair(QString("redirect_uri"), redirectUri));
    query.append(qMakePair(QString("response_type"), QString("code")));
    query.append(qMakePair(QString("client_id"), clientId));
    query.append(qMakePair(QString("access_type"), accessType ? QString("online") : QString("offline")));
    query.append(qMakePair(QString("approval_prompt"), approvalPrompt ? QString("force") : QString("auto")));
    query.append(qMakePair(QString("state"), state));


#if QT_VERSION >= 0x050000
    // Qt5 code
    QUrlQuery urlQuery;
    urlQuery.setQueryItems(query);
    url.setQuery(urlQuery);
#else
    // Qt4 code
    url.setQueryItems(query);
#endif




    return url;
}

QString Auth::getOAuth2Code(const QUrl &url)
{
#if QT_VERSION >= 0x050000
    // Qt5 code
    QUrlQuery urlQuery(url);
    return urlQuery.queryItemValue("code");
#else
    // Qt4 code
    return QString(url.queryItemValue("code"));
#endif
}

void Auth::getTokens(const QString &code, const QString &clientId, const QString &clientSecret, const QString &redirectUri)
{
    currentRequest = EAllTokens;

    postData = "code=";
    postData += code.toLatin1();
    postData += "&client_id=";
    postData += clientId.toLatin1();
    postData += "&client_secret=";
    postData += clientSecret.toLatin1();
    postData += "&redirect_uri=";
    postData += redirectUri.toLatin1();
    postData += "&grant_type=authorization_code";

    performRequest();
}

void Auth::getAccessToken(const QString &clientId, const QString &clientSecret, const QString &refreshToken)
{
    currentRequest = EAccessToken;

    postData = "client_id=";
    postData += clientId.toLatin1();
    postData += "&client_secret=";
    postData += clientSecret.toLatin1();
    postData += "&refresh_token=";
    postData += refreshToken.toLatin1();
    postData += "&grant_type=refresh_token";

    performRequest();
}

void Auth::postFinishedActions(QNetworkReply* reply)
{
    replyStr = reply->readAll();

    JSONParser jParser;

    QString accessToken(jParser.getPlainParam(replyStr, "access_token"));
    QString refreshToken(jParser.getPlainParam(replyStr, "refresh_token"));

    if(currentRequest == EAccessToken)
    {
        emit signalAuthResponse(accessToken);
    }

    if(currentRequest == EAllTokens)
    {
        emit signalAuthResponse(accessToken, refreshToken);
    }
}

void Auth::performRequest(void)
{
    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");
    postRequest(QUrl("https://accounts.google.com/o/oauth2/token"));
}

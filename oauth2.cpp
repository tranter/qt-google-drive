#include "oauth2.h"
#include <QApplication>
#include "logindialog.h"
#include <QSettings>
#include "AppRegData.h"
#include "Def.h"

OAuth2::OAuth2(QWidget* parent) :
   loginDialog(new LoginDialog(parent)),
   networkManager(new QNetworkAccessManager(this))
{ 
    scope = SCOPE;
    clientID = CLIENT_ID;
    redirectURI = REDIRECT_URI;
    endPoint = END_POINT;

    setConnections();

    QSettings settings(COMPANY_NAME, APP_NAME);

    accessToken = settings.value("access_token", "").toString();
    refreshToken = settings.value("refresh_token").toString();
}

OAuth2::~OAuth2()
{
}

void OAuth2::setConnections(void)
{
    connect(loginDialog.data(), SIGNAL(signalCodeObtained()), this, SLOT(slotCodeObtained()));
    connect(networkManager.data(), SIGNAL(finished(QNetworkReply*)),this, SLOT(slotReplyFinished(QNetworkReply*)));
}

void OAuth2::slotCodeObtained()
{
    codeStr = loginDialog->code();
    QUrl url(OAUTH2_TOKEN_URL);

    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    QByteArray params = "client_id=" + QByteArray(CLIENT_ID);
    params += "&redirect_uri=";
    params += QByteArray(REDIRECT_URI);
    params += "&client_secret=";
    params += QByteArray(CLIENT_SECRET);
    params += "&grant_type=authorization_code";
    params += "&code=" + codeStr;

    networkManager->post(request, params);
}

void OAuth2::slotReplyFinished(QNetworkReply* reply)
{
    QSettings settings(COMPANY_NAME, APP_NAME);
    QString replyStr = reply->readAll();

    int expires = getParamFromJson(replyStr, "expires_in").toInt();
    accessToken = getParamFromJson(replyStr, "access_token");

    settings.setValue("access_token", accessToken);

    QString newRefreshToken = getParamFromJson(replyStr, "refresh_token");
    if(!newRefreshToken.isEmpty())
    {
        refreshToken = newRefreshToken;
        settings.setValue("refresh_token", refreshToken);
    }

    if(!accessToken.isEmpty()) QTimer::singleShot((expires - 120) * 1000, this, SLOT(getAccessTokenFromRefreshToken()));

    emit loginDone();
}

QString OAuth2::getParamFromJson(const QString& jsonStr, const QString& lval)
{
    QString optStr = jsonStr;
    optStr.remove(QRegExp("[ \"]"));
    QStringList parseStrs = optStr.split("\n");
    QString rval("");

    for (int i = 0; i < parseStrs.count(); ++i)
    {
        QStringList exp = parseStrs[i].split(",");

        for(int j = 0; j < exp.count(); ++j)
        {
            QStringList token = exp[j].split(":");

            if(token[0] == lval)
            {
                rval = token[1];
                break;
            }
        }
    }

    return rval;
}

void OAuth2::setScope(const QString& scopeStr)
{
    scope = scopeStr;
}

void OAuth2::setClientID(const QString& clientIDStr)
{
    clientID = clientIDStr;
}

void OAuth2::setRedirectURI(const QString& redirectURIStr)
{
    redirectURI = redirectURIStr;
}

QString OAuth2::permanentLoginUrl()
{
    QString str = QString("%1?client_id=%2&redirect_uri=%3&response_type=code&scope=%4&approval_prompt=force&access_type=offline").
            arg(endPoint).arg(clientID).arg(redirectURI).arg(scope);

    return str;
}


bool OAuth2::isAuthorized()
{
    return accessToken.isEmpty();
}

void OAuth2::startLogin(bool bForce)
{
    if(accessToken.isEmpty() || refreshToken.isEmpty() || bForce)
    {
        loginDialog->setLoginUrl(permanentLoginUrl());
        loginDialog->show();
    }
    else
    {
        getAccessTokenFromRefreshToken();
    }
}

void OAuth2::getAccessTokenFromRefreshToken()
{
    QUrl url(OAUTH2_TOKEN_URL);
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    QByteArray params = "client_id=" + QByteArray(CLIENT_ID);
    params += "&client_secret=";
    params += QByteArray(CLIENT_SECRET);
    params += "&grant_type=refresh_token";
    params += "&refresh_token=" + refreshToken;

    networkManager->post(request, params);
}



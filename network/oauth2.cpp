#include "oauth2.h"
#include <QApplication>
#include "gui/forms/logindialog.h"
#include <QSettings>
#include "share/registration.h"
#include "share/defs.h"
#include "parsers/jsonparser.h"
#include "share/debug.h"

OAuth2::OAuth2(QWidget* parent) :
    NetworkManager(parent),
    loginDialog(new LoginDialog(parent))
{ 
    DEBUG;

    scope = SCOPE;
    clientID = CLIENT_ID;
    redirectURI = REDIRECT_URI;
    endPoint = END_POINT;

    QSettings settings(COMPANY_NAME, APP_NAME);

    accessToken = settings.value(ACCESS_TOKEN).toString();
    refreshToken = settings.value(REFRESH_TOKEN).toString();

    init();
    networkManager = getNetworkManager();

    setConnections();
}

OAuth2::~OAuth2()
{
    DEBUG;
}

void OAuth2::setConnections(void)
{
    connect(loginDialog.data(), SIGNAL(signalCodeObtained()), this, SLOT(slotCodeObtained()));
    connect(networkManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotReplyFinished(QNetworkReply*)));
}

void OAuth2::slotCodeObtained()
{
    DEBUG;

    codeStr = loginDialog->code();

    QByteArray params = "client_id=" + QByteArray(CLIENT_ID);

    params += "&redirect_uri=";
    params += QByteArray(REDIRECT_URI);
    params += "&client_secret=";
    params += QByteArray(CLIENT_SECRET);
    params += "&grant_type=authorization_code";
    params += "&code=" + codeStr;

    init();

    networkManager = getNetworkManager();
    setConnections();

    networkManager->post(setRequest(), params);
}

void OAuth2::slotReplyFinished(QNetworkReply* reply)
{
    DEBUG;

    QSettings settings(COMPANY_NAME, APP_NAME);
    QString replyStr = reply->readAll();
    JSONParser jParser;

    int expires = jParser.getParam(replyStr, "expires_in").toInt();
    accessToken = jParser.getParam(replyStr, ACCESS_TOKEN);

    settings.setValue(ACCESS_TOKEN, accessToken);

    QString newRefreshToken = jParser.getParam(replyStr, REFRESH_TOKEN);

    if(!newRefreshToken.isEmpty())
    {
        refreshToken = newRefreshToken;
        settings.setValue(REFRESH_TOKEN, refreshToken);
    }

    if(!accessToken.isEmpty())
    {
        QTimer::singleShot((expires - 120) * 1000/120, this, SLOT(getAccessTokenFromRefreshToken()));
    }

    emit loginDone();
}

void OAuth2::setScope(const QString& scopeStr)
{
    DEBUG;

    scope = scopeStr;
}

void OAuth2::setClientID(const QString& clientIDStr)
{
    DEBUG;

    clientID = clientIDStr;
}

void OAuth2::setRedirectURI(const QString& redirectURIStr)
{
    DEBUG;

    redirectURI = redirectURIStr;
}

QString OAuth2::permanentLoginUrl()
{
    DEBUG;

    QString str = QString("%1?client_id=%2&redirect_uri=%3&response_type=code&scope=%4&approval_prompt=force&access_type=offline").
            arg(endPoint).arg(clientID).arg(redirectURI).arg(scope);

    return str;
}

bool OAuth2::isAuthorized()
{
    DEBUG;

    return accessToken.isEmpty();
}

void OAuth2::startLogin(bool bForce)
{
    DEBUG;

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
    DEBUG;

    QByteArray params = "client_id=" + QByteArray(CLIENT_ID);

    params += "&client_secret=";
    params += QByteArray(CLIENT_SECRET);
    params += "&grant_type=refresh_token";
    params += "&refresh_token=" + refreshToken;

    init();

    networkManager = getNetworkManager();
    setConnections();

    networkManager->post(setRequest(), params);
}

QNetworkRequest OAuth2::setRequest(void)
{
    DEBUG;

    QUrl url(OAUTH2_TOKEN_URL);
    QNetworkRequest request;

    request.setUrl(url);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    return request;
}



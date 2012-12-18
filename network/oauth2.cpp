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
    scope = SCOPE;
    clientID = CLIENT_ID;
    redirectURI = REDIRECT_URI;
    endPoint = END_POINT;

    initAccess();
}

OAuth2::~OAuth2()
{
}

void OAuth2::initAccess(void)
{
    init();
    networkManager = getNetworkManager();
    setConnections();
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

    initAccess();

    networkManager->post(setRequest(), params);
}

void OAuth2::slotReplyFinished(QNetworkReply* reply)
{
    QSettings settings(COMPANY_NAME, APP_NAME);
    QString replyStr = reply->readAll();
    JSONParser jParser;

    DEBUG << "<===============================================================================================================";
    DEBUG << "replyStr" << replyStr;
    DEBUG << "===============================================================================================================>";

    int expiresIn = jParser.getPlainParam(replyStr, TOKEN_EXPIRES_IN).toInt();

    accessToken = jParser.getPlainParam(replyStr, ACCESS_TOKEN);
    settings.setValue(ACCESS_TOKEN, accessToken);

    refreshToken = jParser.getPlainParam(replyStr, REFRESH_TOKEN);

    DEBUG << "accessToken" << accessToken << "refreshToken" << refreshToken << "expiresIn" << expiresIn;

    if(!refreshToken.isEmpty())
    {
        settings.setValue(REFRESH_TOKEN, refreshToken);
    }

    QTimer::singleShot(expiresIn * 1000, this, SLOT(slotGetAccessTokenFromRefreshToken()));

    emit loginDone();
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

QString OAuth2::permanentLoginUrl(void)
{
    QString str = QString("%1?client_id=%2&redirect_uri=%3&response_type=code&scope=%4&approval_prompt=force&access_type=offline").
            arg(endPoint).arg(clientID).arg(redirectURI).arg(scope);

    return str;
}

void OAuth2::startLogin(bool runDialog)
{
    if(runDialog)
    {
        loginDialog->setLoginUrl(permanentLoginUrl());
        loginDialog->show();
    }
    else
    {
        slotGetAccessTokenFromRefreshToken();
    }
}

void OAuth2::slotGetAccessTokenFromRefreshToken(void)
{
    QSettings settings(COMPANY_NAME, APP_NAME);

    accessToken = settings.value(ACCESS_TOKEN).toString();
    refreshToken = settings.value(REFRESH_TOKEN).toString();

    if(refreshToken.isEmpty())
    {
        return;
    }

    QByteArray params(QByteArray("client_id=") + QByteArray(CLIENT_ID));

    params += QByteArray("&client_secret=");
    params += QByteArray(CLIENT_SECRET);
    params += QByteArray("&grant_type=refresh_token");
    params += QByteArray("&refresh_token=") + refreshToken;

    DEBUG << "========================================" << params;

    initAccess();

    networkManager->post(setRequest(), params);
}

QNetworkRequest OAuth2::setRequest(void)
{
    QUrl url(QString("https://accounts.google.com/o/oauth2/token"));
    QNetworkRequest request;

    request.setUrl(url);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    return request;
}



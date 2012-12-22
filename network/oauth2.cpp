#include "oauth2.h"
#include "gui/forms/logindialog.h"
#include "settings/settingsmanager.h"
#include "share/registration.h"
#include "share/defs.h"
#include "parsers/jsonparser.h"
#include "share/debug.h"

OAuth2::OAuth2(QWidget* prnt) :
    NetworkManager(prnt)
{ 
    loginDialog.reset(new LoginDialog(prnt));
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
    connect(loginDialog.data(), SIGNAL(signalCodeObtained(const QString&)), this, SLOT(slotCodeObtained(const QString&)));
    connect(networkManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotReplyFinished(QNetworkReply*)));
}

void OAuth2::slotCodeObtained(const QString &code)
{
    DEBUG << "======================> code" << code;

    QByteArray params;

    params += QByteArray("client_id=");
    params += QByteArray(CLIENT_ID);
    params += QByteArray("&redirect_uri=");
    params += QByteArray(REDIRECT_URI);
    params += QByteArray("&client_secret=");
    params += QByteArray(CLIENT_SECRET);
    params += QByteArray("&grant_type=authorization_code");
    params += QByteArray("&code=");
    params += code.toAscii();

    initAccess();

    networkManager->post(setRequest(), params);
}

void OAuth2::slotReplyFinished(QNetworkReply* reply)
{
    SettingsManager settingsManager;
    QString replyStr = reply->readAll();
    JSONParser jParser;

    settingsManager.setAccessToken(jParser.getPlainParam(replyStr, ACCESS_TOKEN_KEY));

    QString refreshToken(jParser.getPlainParam(replyStr, REFRESH_TOKEN_KEY));

    if(!refreshToken.isEmpty())
    {
        settingsManager.setRefreshToken(refreshToken);
    }

    emit logged();
}

void OAuth2::startLogin(void)
{
    QUrl url(QString("%1?client_id=%2&redirect_uri=%3&response_type=code&scope=%4&approval_prompt=force&access_type=offline").arg(END_POINT).arg(CLIENT_ID).arg(REDIRECT_URI).arg(SCOPE));

    loginDialog->setLoginUrl(url);
    loginDialog->show();
}

void OAuth2::slotGetAccessTokenFromRefreshToken(void)
{
    QString refreshToken(SettingsManager().refreshToken());

    if(refreshToken.isEmpty())
    {
        return;
    }

    QByteArray params;

    params += QByteArray("client_id=");
    params += QByteArray(CLIENT_ID);
    params += QByteArray("&client_secret=");
    params += QByteArray(CLIENT_SECRET);
    params += QByteArray("&grant_type=refresh_token");
    params += QByteArray("&refresh_token=");
    params += refreshToken.toAscii();

    initAccess();

    networkManager->post(setRequest(), params);
}

QNetworkRequest OAuth2::setRequest(void)
{
    QNetworkRequest request;

    request.setUrl(QUrl("https://accounts.google.com/o/oauth2/token"));
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    return request;
}



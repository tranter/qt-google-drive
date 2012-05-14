#include "oauth2.h"
#include <QDebug>
#include <QApplication>
#include "logindialog.h"
#include <QSettings>
#include <QMessageBox>
#include "AppRegData.h"

OAuth2::OAuth2(QWidget* parent)
{ 
    scope = SCOPE;
    clientID = CLIENT_ID;
    redirectURI = REDIRECT_URI;
    companyName = COMPANY_NAME;
    appName =  APP_NAME;

    endPoint = END_POINT;
    responseType = RESPONSE_TYPE;

    loginDialog = new LoginDialog(parent);
    parent = parent;
    connect(loginDialog, SIGNAL(accessTokenObtained()), this, SLOT(accessTokenObtained()));
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

void OAuth2::setCompanyName(const QString& companyNameStr)
{
    companyName = companyNameStr;
}

void OAuth2::setAppName(const QString& appNameStr)
{
    appName = appNameStr;
}

QString OAuth2::loginUrl()
{        
    QString str = QString("%1?client_id=%2&redirect_uri=%3&response_type=%4&scope=%5").arg(endPoint).arg(clientID).
            arg(redirectURI).arg(responseType).arg(scope);
    qDebug() << "Login URL" << str;
    return str;
}

QString OAuth2::getAccessToken() const
{
    return accessToken;
}

bool OAuth2::isAuthorized()
{
    return accessToken.isEmpty();
}

void OAuth2::startLogin(bool bForce)
{
    QSettings settings(companyName, appName);
    QString str = settings.value("access_token", "").toString();

    qDebug() << "OAuth2::startLogin, token from Settings" << str;
    if(clientID == "YOUR_CLIENT_ID_HERE" || redirectURI == "YOUR_REDIRECT_URI_HERE")
    {
        // TODO: change link to wiki page
        QMessageBox::warning(parent, "Warning",
                             "To work with application you need to register your own application in <b>Google</b>.\n"
                             "Learn more from <a href='http://code.google.com/p/qt-google-drive/'>here</a>");
        return;
    }


    if(str.isEmpty() || bForce)
    {
        loginDialog->setLoginUrl(loginUrl());
        loginDialog->show();
    }
    else
    {
        accessToken = str;
        emit loginDone();
    }
}

void OAuth2::accessTokenObtained()
{
    qDebug() << "accessTokenObtained";

    QSettings settings(companyName, appName);
    accessToken = loginDialog->getAccessToken();
    settings.setValue("access_token", accessToken);
    emit loginDone();
}



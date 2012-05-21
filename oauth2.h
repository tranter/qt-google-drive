#ifndef OAUTH2_H
#define OAUTH2_H

//#include <QString>
//#include <QObject>
#include "networkmanager.h"
#include <QTimer>

class LoginDialog;

class OAuth2 : public QObject
{
    Q_OBJECT

public:
    OAuth2(QWidget* parent = 0);
    ~OAuth2();

public:
    bool isAuthorized();
    void startLogin(bool bForce);

    void setScope(const QString& scopeStr);
    void setClientID(const QString& clientIDStr);
    void setRedirectURI(const QString& redirectURIStr);
    QString permanentLoginUrl();

private:
    void setConnections(void);
    QString getParamFromJson(const QString& jsonStr, const QString& lval);

private slots:
    void slotReplyFinished(QNetworkReply* reply);
    void slotCodeObtained();
    void getAccessTokenFromRefreshToken();

signals:
    void loginDone();//Signal that is emitted when login is ended OK.

private:
    QNetworkAccessManager* networkManager;
    QString accessToken;
    QString refreshToken;
    QString endPoint;
    QString scope;
    QString clientID;
    QString redirectURI;
    LoginDialog* loginDialog;
    QString codeStr;
};

#endif // OAUTH2_H

#ifndef OAUTH2_H
#define OAUTH2_H

#include "network/networkmanager.h"
#include <QTimer>

class LoginDialog;

class OAuth2 : public NetworkManager
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
    QNetworkRequest setRequest(void);

private slots:
    void slotReplyFinished(QNetworkReply* reply);
    void slotCodeObtained();
    void getAccessTokenFromRefreshToken();

signals:
    void loginDone();

private:
    QScopedPointer<LoginDialog> loginDialog;
    QNetworkAccessManager* networkManager;

    QString accessToken;
    QString refreshToken;
    QString endPoint;
    QString scope;
    QString clientID;
    QString redirectURI;

    QString codeStr;
};

#endif // OAUTH2_H

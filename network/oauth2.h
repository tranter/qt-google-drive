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
    void startLogin(bool runDialog);
    void setScope(const QString& scopeStr);
    void setClientID(const QString& clientIDStr);
    void setRedirectURI(const QString& redirectURIStr);


private:
    void setConnections(void);
    QNetworkRequest setRequest(void);
    void initAccess(void);
    QString firstLogin(void);

public slots:
    void slotGetAccessTokenFromRefreshToken(void);

private slots:
    void slotReplyFinished(QNetworkReply* reply);
    void slotCodeObtained();


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

#ifndef OAUTH2_H
#define OAUTH2_H

#include "network/networkmanager.h"
#include <QTimer>

class LoginDialog;

class OAuth2 : public NetworkManager
{
    Q_OBJECT
public:
    OAuth2(QWidget* prnt = 0);
    ~OAuth2();

public:
    void startLogin(void);

private:
    void setConnections(void);
    QNetworkRequest setRequest(void);
    void initAccess(void);

public slots:
    void slotGetAccessTokenFromRefreshToken(void);

private slots:
    void slotReplyFinished(QNetworkReply* reply);
    void slotCodeObtained(const QString &code);


signals:
    void logged(void);

private:
    QScopedPointer<LoginDialog> loginDialog;
    QNetworkAccessManager* networkManager;
};

#endif // OAUTH2_H

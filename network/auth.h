#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include "networkmanager.h"

class Auth : public NetworkManager
{
    Q_OBJECT
public:
    enum ERequests
    {
        EAllTokens = 0,
        EAccessToken
    };

    explicit Auth(QObject *parent = 0);

signals:
    void authResponse(const QString &accessToken);
    void authResponse(const QString &accessToken, const QString &refreshToken);

protected:
    virtual void postFinishedActions(QNetworkReply* reply);

public:
    QUrl getOAuth2CodeUrl(const QString &scope, const QString &redirectUri, const QString &clientId , bool accessType = true, bool approvalPrompt = false, const QString &state = QString());
    QString getOAuth2Code(const QUrl &url);
    void getTokens(const QString &code, const QString &clientId, const QString &clientSecret, const QString &redirectUri);
    void getAccessToken(const QString &clientId, const QString &clientSecret, const QString &refreshToken);

private:
    void performRequest(void);

private:
    ERequests currentRequest;
};

#endif // AUTHENTICATION_H

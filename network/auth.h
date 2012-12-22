#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include "networkmanager.h"

class Auth : public NetworkManager
{
    Q_OBJECT
public:
    explicit Auth(QObject *parent = 0);

public:
    QUrl getOAuth2CodeUrl(const QString &scope, const QString &redirectUri, const QString &clientId , bool accessType = true, bool approvalPrompt = false, const QString &state = QString());
    
};

#endif // AUTHENTICATION_H

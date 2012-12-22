#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include "networkmanager.h"

class Auth : public NetworkManager
{
    Q_OBJECT
public:
    explicit Auth(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // AUTHENTICATION_H

#ifndef DRIVEENGINE_H
#define DRIVEENGINE_H

#include <QObject>
#include "oauth2.h"

class DriveEngine : public QObject
{
    Q_OBJECT
public:
    explicit DriveEngine(QObject *parent = 0);
    
signals:
    
public slots:    
    void startLogin(void);

private:
    OAuth2 oAuth2;
    
};

#endif // DRIVEENGINE_H

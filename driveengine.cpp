#include "driveengine.h"
#include <QDebug>

DriveEngine::DriveEngine(QObject *parent) :
    QObject(parent)
{
}

 void DriveEngine::startLogin(void)
 {
     qDebug() << "startLogin";
     oAuth2.startLogin(true);
 }

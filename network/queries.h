#ifndef QUERIES_H
#define QUERIES_H

#include <QObject>
//#include "share/singleton.h"

class Queries : public QObject
{
     Q_OBJECT
public:
    Queries();

public:
//    void setAccountInfo(const QString &accessToken, const QString &refreshToken = QString());

//signals:
//    void signalAccountInfoReadyToUse(void);

//private slots:
//    void slotAccountInfo(AccountInfo::Data &data);

//private:
    //   AccountInfo *accountInfo;
    void userAboutInfo(QString &userInfoQuery, QString &aboutInfoQuery);
};

//typedef TSingleton<Queries> SQueries;

#endif // QUERIES_H

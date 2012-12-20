#ifndef QUERIES_H
#define QUERIES_H

#include <QObject>
#include "settings/accountinfo.h"
#include "share/singleton.h"

class Queries : public QObject
{
     Q_OBJECT
public:
    Queries();

public:
    void setAccountInfo(const QString &accessToken);

signals:
    void signalAccountInfoReadyToUse(void);

private slots:
    void slotAccountInfo(AccountInfo::Data &data);

private:
   AccountInfo *accountInfo;
};

typedef TSingleton<Queries> SQueries;

#endif // QUERIES_H

#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

//#include <QObject>
//#include <QNetworkAccessManager>
//#include <QNetworkReply>
//#include <QSettings>
//#include "AppRegData.h"
//#include <QProgressDialog>
//#include <QFile>
#include "commontools.h"
//#include <QFileInfo>
#include "networkmanager.h"

class DownloadFileManager : public NetworkManager
{
     Q_OBJECT
public:
    explicit DownloadFileManager(QObject *parent = 0);

public:
    virtual void setDownloadSettings(void);

public slots:
    virtual void downloadFinished();

private:
    QString getExt(void) const;
};

#endif // DOWNLOADMANAGER_H

#ifndef RESMANAGER_H
#define RESMANAGER_H

#include "Def.h"
#include "networkmanager.h"
#include "commontools.h"

class ResManager: public NetworkManager
{
     Q_OBJECT
public:
    explicit ResManager(QObject *parent = 0);

public:
    virtual void setStartSettings(QUrl url, const QString& fileName, const QString& progressBarDialogInfoText);
    virtual void setDownloadSettings(void);

public:
    void cashRes(const QString& url);

public slots:
    virtual void downloadFinished();

signals:
    void signalResDownloaded();

private:
    void setResources();

};

#endif // RESMANAGER_H

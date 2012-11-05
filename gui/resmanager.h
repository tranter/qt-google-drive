#ifndef RESMANAGER_H
#define RESMANAGER_H

#include "share/Def.h"
#include "network/networkmanager.h"

class ResManager: public NetworkManager
{
     Q_OBJECT
public:
    explicit ResManager(QObject *parent = 0);

public:
    virtual void setStartSettings(QUrl url, const QString &fileName, const QString &progressBarDialogInfoText);
    virtual void setDownloadSettings(void);

public:
    void cash(const QString &url);

public slots:
    virtual void slotDownloadFinished();

signals:
    void signalResDownloaded();

private:
    void setResources();

};

#endif // RESMANAGER_H

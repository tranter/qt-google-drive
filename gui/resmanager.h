#ifndef RESMANAGER_H
#define RESMANAGER_H

#include "share/defs.h"
#include "network/downloadmanager.h"

class ResManager: public DownloadFileManager
{
     Q_OBJECT
public:
    explicit ResManager(QObject *parent = 0);

public:
    virtual void setProgressBarSettings(QUrl url, const QString &fileName, const QString &progressBarDialogInfoText);
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

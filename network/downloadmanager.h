#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include "share/commontools.h"
#include "networkmanager.h"

class DownloadFileManager : public NetworkManager
{
    Q_OBJECT
public:
    explicit DownloadFileManager(QObject *parent = 0);

public:

    void startDownload(QUrl url, QString &fileName, const QString &type = "text/html");

public slots:
    virtual void slotDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    virtual void slotDownloadFinished();
    virtual void slotDownloadReadyRead();

protected:
    virtual void setDownloadSettings(void);

private:
    QString getExt(void) const;
};

#endif // DOWNLOADMANAGER_H

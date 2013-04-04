#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include "network/networkmanager.h"
#include "network/downloadmanager.h"
#include "operation.h"

class Download : public NetworkManager, public Operation
{
    Q_OBJECT
public:
    explicit Download(QObject *parent = 0);

private slots:
    void slotDownloaded();

public:
    void file(const Items::Data &source, const QString &destination);

private:
    QScopedPointer<DownloadFileManager> downloadMngr;
};

#endif // DOWNLOAD_H

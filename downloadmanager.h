#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include "commontools.h"
#include "networkmanager.h"

class DownloadFileManager : public NetworkManager
{
     Q_OBJECT
public:
    explicit DownloadFileManager(QObject *parent = 0);

public:
    virtual void setDownloadSettings(void);

private:
    QString getExt(void) const;
};

#endif // DOWNLOADMANAGER_H

#ifndef UPLOADMANAGER_H
#define UPLOADMANAGER_H

#include "commontools.h"
#include "networkmanager.h"
#include <QFileInfo>

class UploadFileManager : public NetworkManager
{
    Q_OBJECT
public:  
    explicit UploadFileManager(QObject *parent = 0);
    
public:
    virtual void setUploadSettings(void);
    virtual void setPostFinishedSettings(QNetworkReply* reply);

public:
    virtual void uploadFinished();

private:
    QString getContentTypeByExtension(const QString& ext);

signals:
    void signalUpdateModel();

private:
    bool allowPut;
    
};

#endif // UPLOADMANAGER_H

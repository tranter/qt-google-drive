#ifndef UPLOADMANAGER_H
#define UPLOADMANAGER_H

#include "share/commontools.h"
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
    void startUpload(QUrl url, const QString &fileName);

signals:
    void signalUpdateFileList();

public slots:
    virtual void slotUploadFinished();
    virtual void slotUploadProgress(qint64 bytesSent, qint64 bytesTotal);

private:
    QString getContentTypeByExtension(const QString &ext);

private:
    bool allowPut;
};

#endif // UPLOADMANAGER_H

#ifndef UPLOADMANAGER_H
#define UPLOADMANAGER_H

#include <QObject>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include "AppRegData.h"
#include <QProgressDialog>
#include <QFile>
#include "commontools.h"

class UploadFileManager : public QObject
{
    Q_OBJECT
public:
    enum EStates
    {
        EReady = 0,
        EBusy
    };

    explicit UploadFileManager(QObject *parent = 0);
    
    ~UploadFileManager();

public:
    void startUpload(const QString& fileName);
    EStates getState(void) const;
    void setState(EStates currentState);

public slots:
    void uploadProgress (qint64 bytesSent, qint64 bytesTotal);
    void uploadFinished();
    void uploadReadyRead();

private:
    QNetworkAccessManager* networkManager;
    QNetworkReply *reply;
    QNetworkRequest request;
    QString accessToken;
    QProgressDialog progressDialog;
    QFile file;
    EStates state;
    
};

#endif // UPLOADMANAGER_H

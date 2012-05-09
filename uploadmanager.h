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
#include <QFileInfo>
#include <QSslError>

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
    void startUpload(const QString& fileName, QUrl uploadUrl, const QString& accessToken);
    EStates getState(void) const;
    void setState(EStates currentState);

private:
    QString getContentTypeByExtension(const QString& ext);
    void doPutRequest(const QString & url,const QByteArray& data);

public slots:
    void uploadProgress (qint64 bytesSent, qint64 bytesTotal);
    void uploadFinished();
    void slotError(QNetworkReply::NetworkError error);
    void slotSslErrors(const QList<QSslError>& errors);
    void postFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager* networkManager;
    QNetworkReply *reply;
    QNetworkRequest request;
    QString accessToken;
    QProgressDialog progressDialog;
    QFile file;
    EStates state;
    QString access_token;
    
};

#endif // UPLOADMANAGER_H

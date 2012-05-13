#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProgressDialog>
#include <QFile>
#include <QSslError>

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    enum EStates
    {
        EReady = 0,
        EBusy
    };

    explicit NetworkManager(QObject *parent = 0);
    virtual ~NetworkManager();
    
public:
    virtual void setDownloadSettings(void);
    virtual void setUploadSettings(void);
    virtual void setPostFinishedSettings(QNetworkReply* reply);

public:
    void startDownload(QUrl url, QString& fileName, const QString& fileType = "text/html");
    void startUpload(QUrl url, const QString& fileName);
    void doPutRequest(const QString & url,const QByteArray& data);

public:
    virtual void setStartSettings(QUrl url);

public:
    EStates getState(void) const;
    void setState(EStates currentState);

public slots:
    virtual void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    virtual void downloadFinished();
    virtual void downloadReadyRead();
    virtual void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
    virtual void uploadFinished();
    virtual void slotError(QNetworkReply::NetworkError error);
    virtual void slotSslErrors(const QList<QSslError>& errors);
    virtual void postFinished(QNetworkReply* reply);

protected:
    QNetworkAccessManager* networkManager;
    QNetworkReply *reply;
    QNetworkRequest request;
    QByteArray uploadContent;
    QProgressDialog progressDialog;
    EStates state;
    QFile file;
    QString type;
};

#endif // NETWORKMANAGER_H




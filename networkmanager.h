#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QSslError>
#include "progressbardialog.h"
#include <QFileInfo>

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
    void startDownload(QUrl url, QString& fileName, const QString& type = "text/html");
    void startUpload(QUrl url, const QString& fileName);
    void putRequest(const QString & url,const QByteArray& data);
    void getRequest(const QString & url);

public:
    virtual void setStartSettings(QUrl url, const QString& fileName, const QString& progressBarDialogInfoText);

public:
    EStates getState(void) const;
    void setState(EStates currentState);
    const NetworkManager* self(void) const;

public slots:
    virtual void slotDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    virtual void slotDownloadFinished();
    virtual void slotDownloadReadyRead();
    virtual void slotUploadProgress(qint64 bytesSent, qint64 bytesTotal);
    virtual void slotUploadFinished();
    virtual void slotPostFinished(QNetworkReply* reply);
    virtual void slotError(QNetworkReply::NetworkError error);
    virtual void slotSslErrors(const QList<QSslError>& errors);
    virtual void slotReplyFinished(QNetworkReply* reply);
    virtual void slotReplyReadyRead();
    virtual void slotProgressCanceled();

private:
    void connectErrorHandlers(void);

protected:
    QObject* parent;
    QNetworkAccessManager* networkManager;
    QNetworkReply *reply;
    QNetworkRequest request;
    QByteArray uploadContent;
    EStates state;
    QFile file;
    QString fileType;
    ProgressBarDialog progressBarDialog;
    bool operationCanceled;
    QString replyStr;
};

#endif // NETWORKMANAGER_H




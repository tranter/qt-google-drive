#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QSslError>
#include "gui/forms/progressbardialog.h"
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
    
signals:
    void signalAccessTokenRequired();

public:
    //virtual void setDownloadSettings(void);
    //virtual void setUploadSettings(void);
    virtual void setPostFinishedSettings(QNetworkReply*);

public:
    QNetworkAccessManager *getNetworkManager(void) const;
    QNetworkRequest getRequest(void) const;
    void init(void);
    void putRequest(const QString &url,const QByteArray &data);
    void sendRequest(const QString &url);
    //void startDownload(QUrl url, QString &fileName, const QString &type = "text/html");
    //void startUpload(QUrl url, const QString &fileName);

public:
    virtual void setStartSettings(QUrl url, const QString &fileName, const QString &progressBarDialogInfoText);

public:
    EStates getState(void) const;
    void setState(EStates currentState);
    const NetworkManager* self(void) const;
    void connectErrorHandlers(void);

public slots:
//    virtual void slotDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
//    virtual void slotDownloadFinished();
//    virtual void slotDownloadReadyRead();
//    virtual void slotUploadProgress(qint64 bytesSent, qint64 bytesTotal);
//    virtual void slotUploadFinished();
    virtual void slotPostFinished(QNetworkReply* reply);
    virtual void slotError(QNetworkReply::NetworkError error);
    virtual void slotSslErrors(const QList<QSslError> &errors);
    virtual void slotReplyFinished(QNetworkReply*);
    virtual void slotReplyReadyRead();
    virtual void slotProgressCanceled();

protected:
    QObject* parent;
    QScopedPointer <QNetworkAccessManager> networkManager;
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




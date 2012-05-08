#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include "AppRegData.h"
#include <QDesktopServices>
#include <QProgressDialog>
#include <QFile>
#include "commontools.h"

class DownloadFileManager : public QObject
{
    Q_OBJECT
public:

    enum EStates
    {
        EReady = 0,
        EBusy
    };

    explicit DownloadFileManager(QObject *parent = 0);
    ~DownloadFileManager();

public:
    void startDownload(QUrl url, const QString& fileName);
    EStates getState(void) const;
    void setState(EStates currentState);

public slots:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();
    void downloadReadyRead();

private:
    QNetworkAccessManager* networkManager;
    QNetworkReply *reply;
    QNetworkRequest request;
    QString accessToken;
    QProgressDialog progressDialog;
    QFile file;
    EStates state;

};

#endif // DOWNLOADMANAGER_H

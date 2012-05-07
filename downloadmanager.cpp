#include "downloadmanager.h"
#include <QDebug>

DownloadFileManager::DownloadFileManager(QObject *parent) :
    QObject(parent),
    replyString("")
{
    qDebug() << "DownloadFileManager";
}

DownloadFileManager::~DownloadFileManager()
{
    qDebug() << "~DownloadFileManager";
}

void DownloadFileManager::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug() << "bytesReceived =" << bytesReceived << "bytesTotal =" << bytesTotal;
}

void DownloadFileManager::downloadFinished()
{
    qDebug() << "downloadFinished replyString = " << replyString;
}

void DownloadFileManager::downloadReadyRead()
{
    replyString + reply->readAll();
    qDebug() << "downloadReadyRead";
}

void DownloadFileManager::startDownload(QUrl url)
{
    setHeader(request);
    //request.setUrl(url);
//    request.setUrl(QUrl("https://doc-08-3c-docs.googleusercontent.com/docs/securesc/gqbtlt21m0n8qfluoko65rhjsvkedh7k/kviek32pd2lukjerskrhqj7smoc27sof/1336377600000/01936898393326665359/01936898393326665359/0B_pGaTf6anqmeHJYY1dXVWVaZEU"));

//    qDebug() << "url = " << request.url();

//    reply = networkManager.get(request);


//    QByteArray data;
//    data.append("?&%20%20e=download");

//    QUrl dataPart;
//    dataPart.setEncodedUrl(data);

//    reply = networkManager.post(request, data);


//    connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()));
//    connect(reply, SIGNAL(readyRead()), this, SLOT(downloadReadyRead()));
//    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));

    QDesktopServices::openUrl(url);
}

void DownloadFileManager::setHeader(QNetworkRequest& request)
{
    QSettings settings(COMPANY_NAME, APP_NAME);
    accessToken = settings.value("access_token").toString();

    request.setRawHeader("User-Agent", "Google Drive client");
    request.setRawHeader("GData-Version", "3.0");
    request.setRawHeader("Authorization",(QString("OAuth %1").arg(accessToken)).toLatin1());
}


#include "downloadmanager.h"
#include <QDebug>

DownloadFileManager::DownloadFileManager(QObject *parent) :
    networkManager(new QNetworkAccessManager),
    QObject(parent),
    replyString("")
{
    qDebug() << "DownloadFileManager";
}

DownloadFileManager::~DownloadFileManager()
{
    qDebug() << "~DownloadFileManager";
    if(networkManager) delete networkManager;
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

    request.setUrl(url);

    connect(networkManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));

    qDebug() << "url = " << request.url();

    reply = networkManager->get(request);


    connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(downloadReadyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));

    QDesktopServices::openUrl(url);
}

void DownloadFileManager::replyFinished(QNetworkReply* reply)
{

}

void DownloadFileManager::setHeader(QNetworkRequest& request)
{
    QSettings settings(COMPANY_NAME, APP_NAME);
    accessToken = settings.value("access_token").toString();

    request.setRawHeader("User-Agent", "Google Drive client");
    request.setRawHeader("GData-Version", "3.0");
    request.setRawHeader("Authorization",(QString("OAuth %1").arg(accessToken)).toLatin1());
}


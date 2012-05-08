#include "downloadmanager.h"
#include <QDebug>

DownloadFileManager::DownloadFileManager(QObject *parent) :
    networkManager(new QNetworkAccessManager),
    QObject(parent)
{
}

DownloadFileManager::~DownloadFileManager()
{
    if(networkManager) delete networkManager;
}

void DownloadFileManager::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug() << "bytesReceived =" << bytesReceived << "bytesTotal =" << bytesTotal;
    progressDialog.setMaximum(bytesTotal);
    progressDialog.setValue(bytesReceived);
}

void DownloadFileManager::downloadFinished()
{
 progressDialog.hide();
 file.write(bytes);
}

void DownloadFileManager::downloadReadyRead()
{
    bytes += reply->readAll();
}

void DownloadFileManager::startDownload(QUrl url, const QString& fileName)
{
    file.setFileName(fileName);
    file.open(QIODevice::WriteOnly);

    qDebug() << "fileName = " << fileName;

    progressDialog.show();
    CommonTools::setHeader(request);
    request.setUrl(url);

    qDebug() << "url = " << request.url();

    reply = networkManager->get(request);

    connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(downloadReadyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
}


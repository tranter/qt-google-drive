#include "downloadmanager.h"
#include <QDebug>

DownloadFileManager::DownloadFileManager(QObject *parent) :   
    QObject(parent),
    networkManager(new QNetworkAccessManager),
    state(EReady)
{
    progressDialog.setParent(static_cast<QWidget*>(parent));
    progressDialog.setCancelButton(0);
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
 state = EReady;
}

void DownloadFileManager::downloadReadyRead()
{
    file.write(reply->readAll());
}

void DownloadFileManager::startDownload(QUrl url, const QString& fileName)
{
    state = EBusy;

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

DownloadFileManager::EStates DownloadFileManager::getState(void) const
{
  return state;
}

void DownloadFileManager::setState(DownloadFileManager::EStates currentState)
{
 state = currentState;
}


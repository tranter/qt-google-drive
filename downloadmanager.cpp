#include "downloadmanager.h"
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    file.flush();
    file.close();

    UiInstance::ui->actionMenuDownload->setEnabled(true);
    UiInstance::ui->actionDownload->setEnabled(true);
}

void DownloadFileManager::downloadReadyRead()
{
    file.write(reply->readAll());
}

void DownloadFileManager::startDownload(QUrl url, QString& fileName, const QString& fileType)
{
    state = EBusy;

    QFileInfo fi(fileName);
    QString ext = fi.suffix();

    if(ext.isEmpty()) fileName += getExt(fileType);

    file.setFileName(fileName);
    file.open(QIODevice::WriteOnly);

    progressDialog.show();
    CommonTools::setHeader(request);
    request.setUrl(url);

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

QString DownloadFileManager::getExt(const QString& fileType) const
{
    QString ext(".html");

    if(fileType == "text/html") ext = ".html";
    if(fileType == "image/png") ext = ".png";
    if(fileType == "application/pdf") ext = ".pdf";

    return ext;
}


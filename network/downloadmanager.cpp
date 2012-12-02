#include "downloadmanager.h"
#include "gui/forms/mainwindow.h"
#include "ui_mainwindow.h"

DownloadFileManager::DownloadFileManager(QObject *parent) :
    NetworkManager(parent)
{
}

void DownloadFileManager::setDownloadSettings(void)
{
    QFileInfo fi(file.fileName());
    QString ext = fi.suffix();

    if(ext.isEmpty())
    {
        file.setFileName(file.fileName() + getExt());
    }

    CommonTools::setHeader(request);
}

QString DownloadFileManager::getExt(void) const
{
    QString ext(".html");

    if(fileType == "text/html") ext = ".html";
    if(fileType == "image/png") ext = ".png";
    if(fileType == "application/pdf") ext = ".pdf";

    return ext;
}

void DownloadFileManager::startDownload(QUrl url, QString& fileName, const QString& type)
{
    init();

    fileType = type;

    setProgressBarSettings(url, fileName, "Downloading file: ");
    setDownloadSettings();

    file.open(QIODevice::WriteOnly);

    reply = networkManager->get(request);

    connect(reply, SIGNAL(finished()), this, SLOT(slotDownloadFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(slotDownloadReadyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(slotDownloadProgress(qint64,qint64)));

    connectErrorHandlers();
}

void DownloadFileManager::slotDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    progressBarDialog.setMaximum(bytesTotal);
    progressBarDialog.setValue(bytesReceived);
}

void DownloadFileManager::slotDownloadFinished()
{
    progressBarDialog.hide();

    state = EReady;

    file.flush();
    file.close();
}

void DownloadFileManager::slotDownloadReadyRead()
{
    file.write(reply->readAll());
}

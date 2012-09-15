#include "resmanager.h"

ResManager::ResManager(QObject *parent):
            NetworkManager(parent)
{
}

void ResManager::cashRes(const QString& url)
{
  QFileInfo fi(url);
  QString fileNameToSave(fi.fileName());
  NetworkManager::startDownload(url, fileNameToSave, "image/png");
}

void ResManager::setDownloadSettings(void)
{
    CommonTools::setHeader(request);
}

void ResManager::setStartSettings(QUrl url, const QString& fileName, const QString& progressBarDialogInfoText)
{
    state = EBusy;
    file.setFileName(fileName);
    request.setUrl(url);
}

void ResManager::slotDownloadFinished()
{
    NetworkManager::slotDownloadFinished();
    emit signalResDownloaded();
}

#include "resmanager.h"
#include "share/commontools.h"

ResManager::ResManager(QObject *parent):
            DownloadFileManager(parent)
{
}

void ResManager::cash(const QString &url)
{
  QFileInfo fi(url);
  QString fileNameToSave(fi.fileName());

  startDownload(url, fileNameToSave, "image/png");
}

void ResManager::setDownloadSettings(void)
{
    CommonTools::setHeader(request);
}

void ResManager::setProgressBarSettings(QUrl url, const QString &fileName, const QString &progressBarDialogInfoText)
{
    Q_UNUSED(progressBarDialogInfoText);
    state = EBusy;
    file.setFileName(fileName);
    request.setUrl(url);
}

void ResManager::slotDownloadFinished()
{
    DownloadFileManager::slotDownloadFinished();
    emit signalResDownloaded();
}

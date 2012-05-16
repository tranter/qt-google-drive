#include "resmanager.h"
#include <QDebug>

ResManager::ResManager(QObject *parent):
            NetworkManager(parent)
{
}

void ResManager::cashRes(const QString& url)
{
  QFileInfo fi(url);
  QString fileNameToSave("res/" + fi.fileName());
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

bool ResManager::remove(void)
{
    qDebug() << "========================================================== " << getState();

    //while(getState() == NetworkManager::EBusy){qDebug() << "NetworkManager::EBusy";}

    return true;
}

void ResManager::downloadFinished()
{
    NetworkManager::downloadFinished();
    emit signalResDownloaded();
}




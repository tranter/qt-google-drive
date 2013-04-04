#include "download.h"
#include <QDir>

Download::Download(QObject *parent) :
    NetworkManager(parent),
    Operation(EDownload)
{
}

void Download::file(const Items::Data &source, const QString &destination)
{
    QString fileName = destination + QDir::toNativeSeparators("/") + source.name;

    downloadMngr.reset(new DownloadFileManager());
    downloadMngr->startDownload(QUrl(source.downloadLink), fileName);

    connect(downloadMngr.data(), SIGNAL(downloaded()), this, SLOT(slotDownloaded()));
}

void Download::slotDownloaded()
{
    updatePanelContent(true);
}

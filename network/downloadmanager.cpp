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

    if(ext.isEmpty()) file.setFileName(file.fileName() + getExt());

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


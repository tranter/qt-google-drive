#include "downloadmanager.h"
#include <QDebug>
#include "mainwindow.h"
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

    if(type == "text/html") ext = ".html";
    if(type == "image/png") ext = ".png";
    if(type == "application/pdf") ext = ".pdf";

    return ext;
}


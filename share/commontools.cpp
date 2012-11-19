#include "commontools.h"
#include "gui/forms/mainwindow.h"
#include "ui_mainwindow.h"
#include "share/registration.h"
#include "share/defs.h"
#include <QLocale>
#include <QSettings>
#include <QMessageBox>
#include <QList>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

void CommonTools::setHeader(QNetworkRequest &request)
{
    QSettings settings(COMPANY_NAME, APP_NAME);
    QString accessToken = settings.value("access_token").toString();

    request.setRawHeader("User-Agent", APP_NAME);
    request.setRawHeader("GData-Version", "3.0");
    request.setRawHeader("Authorization",(QString("OAuth %1").arg(accessToken)).toLatin1());
}

void CommonTools::msg(const QString &text, QWidget *parent)
{
    QMessageBox msgBox(parent);

    msgBox.setText(text);
    msgBox.exec();
}

int CommonTools::errorMsg(const QString &caption, const QString &text, QWidget *parent)
{
    return QMessageBox::critical(parent, caption, text, QMessageBox::Ok);
}

QString CommonTools::convertDate(const QString &dateStr)
{
    QDateTime fileDateTime = QDateTime::fromString(dateStr, "yyyy-MM-ddThh:mm:ss.zzzZ");
    return CommonTools::getFormattedDate(fileDateTime);
}

QString CommonTools::getFormattedDate(QDateTime &dateTime)
{
    QString formattedDateStr;

    dateTime.setTimeSpec(Qt::UTC);

    if(dateTime.date() == QDate::currentDate()) formattedDateStr = dateTime.toLocalTime().toString("h:mm ap");
    else if(dateTime.date().year() == QDate::currentDate().year()) formattedDateStr = dateTime.toLocalTime().toString("MMM d");
    else if(dateTime.date().year() < QDate::currentDate().year()) formattedDateStr = dateTime.toLocalTime().toString("M/d/yy");

    return formattedDateStr;
}

QString CommonTools::getFormattedFileSize(const QString &sizeStr)
{
    qlonglong size = sizeStr.toLongLong();
    QLocale locale;
    QString bytesStr;
    QStringList bytesStrList;
    bytesStrList  << " KB" << " MB" << " GB";
    qlonglong sizes[3] = {1024, 1048576, 1073741824};

    if(size <= sizes[0])
    {
        size = 1;
        bytesStr = bytesStrList[0];
        return locale.toString(size) + bytesStr;
    }

    for(int i = bytesStrList.count() - 1 ; i >= 0 ; --i)
    {
        if (size > sizes[i])
        {
            size /= sizes[i];
            bytesStr = bytesStrList[i];
            break;
        }
    }

    return locale.toString(size) + bytesStr;
}

void CommonTools::logToFile(const QString &fileName, const QByteArray &bytes)
{
    QFile file(fileName);

    file.open(QIODevice::WriteOnly);
    file.write(bytes);
    file.close();
}

QByteArray CommonTools::loadFromFile(const QString &fileName)
{
    QFile file(fileName);

    file.open(QIODevice::ReadOnly);
    QByteArray arr = file.readAll();
    file.close();

    return arr;
}

bool CommonTools::fileFromURLExists(const QString &url)
{
    QFileInfo fi(url);
    QFile file(fi.fileName());

    return file.exists();
}

QString CommonTools::getFileNameFromURL(const QString &url)
{
  QFileInfo fi(url);
  return fi.fileName();
}

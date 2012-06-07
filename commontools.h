#ifndef COMMONTOOLS_H
#define COMMONTOOLS_H

#include <QNetworkRequest>
#include <QSettings>
#include <QMessageBox>
#include "AppRegData.h"
#include <QDateTime>
#include <QList>
#include <QFile>
#include <QFileInfo>

class CommonTools
{
public:
    static void setHeader(QNetworkRequest& request);
    static void msg(const QString& str);
    static QString convertDate(const QString& dtStr);
    static QString getFormattedDate(QDateTime& dt);
    static QString getFormattedFileSize(const QString& sizeStr);
    static void logToFile(const QString& fileName, QByteArray bytes);
    static QByteArray loadFromFile(const QString& fileName);
    static bool fileFromURLExists(const QString& url);
    static QString getFileNameFromURL(const QString& url);
};

#endif // COMMONTOOLS_H

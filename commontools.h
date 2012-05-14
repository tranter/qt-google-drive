#ifndef COMMONTOOLS_H
#define COMMONTOOLS_H

#include <QNetworkRequest>
#include <QSettings>
#include <QMessageBox>
#include "AppRegData.h"
#include <QDateTime>
#include <QList>
#include <QFile>

class CommonTools
{
public:
    static void setHeader(QNetworkRequest& request);
    static void msg(const QString& str);
    static const QList<int> getTreeViewOpenedItem(void);
    static void addTreeViewOpenedItem(int index);
    static void removeTreeViewOpenedItem(int index);
    static void treeViewOpenedItemClear(void);
    static QString convertDate(const QString& dtStr);
    static QString getFormattedDate(QDateTime& dt);
    static QString getFormattedFileSize(const QString& sizeStr);
    static void logToFile(const QString& fileName, QByteArray bytes);

private:
    static QList<int> indexes;
};

#endif // COMMONTOOLS_H

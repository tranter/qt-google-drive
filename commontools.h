#ifndef COMMONTOOLS_H
#define COMMONTOOLS_H

#include <QNetworkRequest>
#include <QSettings>
#include <QMessageBox>
#include "AppRegData.h"

class CommonTools
{
public:

    static void setHeader(QNetworkRequest& request);
    static void msg(const QString& str);
};

#endif // COMMONTOOLS_H

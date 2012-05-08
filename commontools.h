#ifndef COMMONTOOLS_H
#define COMMONTOOLS_H

#include <QNetworkRequest>
#include <QSettings>
#include "AppRegData.h"

class CommonTools
{
public:

    static void setHeader(QNetworkRequest& request);
};

#endif // COMMONTOOLS_H

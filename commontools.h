#ifndef COMMONTOOLS_H
#define COMMONTOOLS_H

#include <QNetworkRequest>
#include <QSettings>
#include <QMessageBox>
#include "AppRegData.h"
//#include <QModelIndex>
#include <QList>

class CommonTools
{
public:
    static void setHeader(QNetworkRequest& request);
    static void msg(const QString& str);
    static const QList<int> getTreeViewOpenedItem(void);
    static void addTreeViewOpenedItem(int index);
    static void removeTreeViewOpenedItem(int index);
    static void treeViewOpenedItemClear(void);

private:
    static QList<int> indexes;
};

#endif // COMMONTOOLS_H

#ifndef TREEITEMINFO_H
#define TREEITEMINFO_H

#include <QVariant>
#include "treeitem.h"

class TreeItemInfo
{
public:
    TreeItemInfo();

    struct Data
    {
        TreeItem *item;
        QVariant name;
        QVariant self;
        QVariant parent;
    };

public:
    void add(QVariant name, QVariant self, QVariant parent);
    void setItemPointer(int index, TreeItem *item);
    void showAll(void);

public:
    QList<Data> items;

//private:

};

#endif // TREEITEMINFO_H

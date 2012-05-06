#ifndef TREEITEMINFO_H
#define TREEITEMINFO_H

#include <QVariant>
#include "treeitem.h"
#include "Def.h"

class TreeItemInfo
{
public:
    TreeItemInfo();

    struct Data
    {
        TreeItem *item;
        QString type;
        QVariant name;
        QVariant self;
        QVariant parent;
        QString iconPath;
    };

public:
    void add(QVariant name, QVariant self, QVariant parent);
    void setItemPointer(int index, TreeItem *item);
    void showAll(void);

public:
    QList<Data> items;



};

#endif // TREEITEMINFO_H

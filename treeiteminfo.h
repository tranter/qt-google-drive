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
    void setPointer(int index, TreeItem *item);
    void push_back(TreeItemInfo::Data& data);
    const QList<TreeItemInfo::Data>& getItems(void) const;

private:
    QList<Data> items;

};

#endif // TREEITEMINFO_H

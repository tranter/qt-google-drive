#ifndef TREEITEMINFO_H
#define TREEITEMINFO_H

#include <QVariant>

class TreeItemInfo
{
public:
    TreeItemInfo();

    struct Data
    {
        QVariant name;
        QVariant self;
        QVariant parent;
    };

    QList<Data> items;
};

#endif // TREEITEMINFO_H

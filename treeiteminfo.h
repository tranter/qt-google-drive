#ifndef TREEITEMINFO_H
#define TREEITEMINFO_H

#include <QVariant>
#include "treeitem.h"
#include "Def.h"

class TreeItemInfo
{
public:
    TreeItemInfo();

    enum ETypes
    {
      Efolder = 0,
      EFile
    };

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
    void push_back(TreeItemInfo::Data& data, ETypes type);
    const QList<TreeItemInfo::Data>& getItems(void) const;
    void normalize(void);

private:
    QList<Data> items;
    QList<Data> fileItems;

};

#endif // TREEITEMINFO_H

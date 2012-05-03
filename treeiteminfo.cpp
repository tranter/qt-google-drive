#include "treeiteminfo.h"
#include <QDebug>

TreeItemInfo::TreeItemInfo()
{
}

void TreeItemInfo::add(QVariant name, QVariant self, QVariant parent)
{
    Data itemData;

    itemData.item = NULL;
    itemData.name = name;
    itemData.self = self;
    itemData.parent = parent;

    items.push_back(itemData);
}

void TreeItemInfo::setItemPointer(int index, TreeItem *item)
{
    items[index].item = item;
}

void TreeItemInfo::showAll(void)
{
    qDebug() << endl;

    for(int i = 0;i < items.count();++i)
        qDebug() << "items[" << i << "].name = " << items[i].name << "items[" << i << "].parent = " << items[i].parent << "items[" << i << "].self = " << items[i].self;
}

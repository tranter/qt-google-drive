#include "treeiteminfo.h"
#include <QDebug>

TreeItemInfo::TreeItemInfo()
{
}

void TreeItemInfo::setItemPointer(int index, TreeItem *item)
{
    items[index].item = item;
}

const QList<TreeItemInfo::Data>& TreeItemInfo::getItems(void) const
{
    return items;
}

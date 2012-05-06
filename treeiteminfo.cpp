#include "treeiteminfo.h"
#include <QDebug>

TreeItemInfo::TreeItemInfo()
{
}

void TreeItemInfo::setPointer(int index, TreeItem *item)
{
    items[index].item = item;
}


const QList<TreeItemInfo::Data>& TreeItemInfo::getItems(void) const
{
    return items;
}

void TreeItemInfo::push_back(TreeItemInfo::Data& data)
{
  items.push_back(data);
}

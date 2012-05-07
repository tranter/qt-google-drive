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

void TreeItemInfo::push_back(TreeItemInfo::Data& data, ETypes type)
{
    switch(type)
    {
    case Efolder: items.push_back(data);
        break;
    case EFile: fileItems.push_back(data);
        break;
    }
}

void TreeItemInfo::normalize(void)
{
    for(int i = fileItems.count() - 1; i > 0; --i)
        items.push_front(fileItems[i]);
}

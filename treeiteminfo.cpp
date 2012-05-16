#include "treeiteminfo.h"
#include <QDebug>

TreeItemInfo::TreeItemInfo()
{
}

void TreeItemInfo::setPointer(int index, TreeItem *item)
{
    items[index].item = item;
}

void TreeItemInfo::setFileSize(const QString& size, int index)
{
    fileItems[index].fileSize = size;
}

const QList<TreeItemInfo::Data>& TreeItemInfo::getItems(void) const
{
    return items;
}

const QList<TreeItemInfo::Data>& TreeItemInfo::getFileItems(void) const
{
    return fileItems;
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
//    for(int i = fileItems.count() - 1; i > 0; --i)
//        items.push_front(fileItems[i]);

    for(int i =  1; i < fileItems.count(); ++i)
        items.push_back(fileItems[i]);
}

TreeItemInfo::Data& TreeItemInfo::operator [] (int index)
{
    return items[index];
}

#include "iteminfo.h"
#include <QtAlgorithms>

ItemInfo::ItemInfo()
{
}

void ItemInfo::setFileSize(const QString &size, int index)
{
    items[index].fileSize = size;
}

const QList<ItemInfo::Data> &ItemInfo::getItems() const
{
    return items;
}

void ItemInfo::sort(QList<ItemInfo::Data> &sortItems, Data::ESortOrder itemSortOrder, Qt::SortOrder sortOrder)
{
    setItemsSortOrder(sortItems, itemSortOrder);

    if(itemSortOrder == Data::ETypeName)
    {
        sortByTypeName(sortItems, sortOrder);
    }
    else
    {
        if(sortOrder == Qt::AscendingOrder) qSort(sortItems.begin(), sortItems.end());
        if(sortOrder == Qt::DescendingOrder) qSort(sortItems.begin(), sortItems.end(), qGreater<ItemInfo::Data>());
    }
}

void ItemInfo::setItemsSortOrder(QList<ItemInfo::Data> &sortItems, Data::ESortOrder itemSortOrder)
{
    for(int i = 0; i < sortItems.count(); ++i)
    {
        sortItems[i].setSortOrder(itemSortOrder);
    }
}

void ItemInfo::sortByTypeName(QList<ItemInfo::Data> &sortItems, Qt::SortOrder sortOrder)
{
    QList<ItemInfo::Data> folderItems, fileItems;

    for(int i = 0; i < sortItems.count(); ++i)
    {
        if(sortItems[i].type == FOLDER_TYPE_STR) folderItems.push_back(sortItems[i]);
        if(sortItems[i].type == FILE_TYPE_STR) fileItems.push_back(sortItems[i]);
    }

    if(sortOrder == Qt::AscendingOrder)
    {
        qSort(folderItems.begin(), folderItems.end());
        qSort(fileItems.begin(), fileItems.end());
    }

    if(sortOrder == Qt::DescendingOrder)
    {
        qSort(folderItems.begin(), folderItems.end(), qGreater<ItemInfo::Data>());
        qSort(fileItems.begin(), fileItems.end(), qGreater<ItemInfo::Data>());
    }

    for(int i = 0; i < folderItems.count(); ++i)
    {
        sortItems[i] = folderItems[i];
    }

    for(int i = folderItems.count(), j = 0; i < sortItems.count(); ++i, ++j)
    {
        sortItems[i] = fileItems[j];
    }
}

void ItemInfo::pushBack(ItemInfo::Data &data)
{
    items.push_back(data);
}

void ItemInfo::setAccountOwner(const QString &name)
{
    accountOwner = name;
}

QString ItemInfo::getAccountOwner(void) const
{
    return accountOwner;
}

void ItemInfo::setDataOwner(const QString &name, int index)
{
    items[index].dataOwner = name;
}


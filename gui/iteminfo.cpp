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
    sortItems = getSortItems(itemSortOrder);

    if(sortOrder == Qt::AscendingOrder) qSort(sortItems.begin(), sortItems.end());
    if(sortOrder == Qt::DescendingOrder) qSort(sortItems.begin(), sortItems.end(), qGreater<ItemInfo::Data>());
}

QList<ItemInfo::Data> ItemInfo::getSortItems(Data::ESortOrder sortOrder)
{
    for(int i = 0; i < items.count(); ++i)
    {
        items[i].setSortOrder(sortOrder);
    }

    return items;
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


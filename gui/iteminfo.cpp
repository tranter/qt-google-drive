#include "iteminfo.h"

ItemInfo::ItemInfo()
{
}

void ItemInfo::setFileSize(const QString &size, int index)
{
    fileItems[index].fileSize = size;
}

const QList<ItemInfo::Data>& ItemInfo::getItems(void) const
{
    return fileItems;
}

void ItemInfo::push_back(ItemInfo::Data &data)
{
    fileItems.push_back(data);
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
    fileItems[index].dataOwner = name;
}


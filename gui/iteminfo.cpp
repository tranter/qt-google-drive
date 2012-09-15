#include "iteminfo.h"

ItemInfo::ItemInfo()
{
}

void ItemInfo::setPointer(int index, QTreeWidgetItem* pointer)
{
    items[index].pointer = pointer;
}

void ItemInfo::setFileSize(const QString& size, int index)
{
    fileItems[index].fileSize = size;
}

const QList<ItemInfo::Data>& ItemInfo::getItems(void) const
{
    return items;
}

const QList<ItemInfo::Data>& ItemInfo::getFileItems(void) const
{
    return fileItems;
}

void ItemInfo::push_back(ItemInfo::Data& data, int type)
{
    switch(type)
    {
    case FOLDER_TYPE: items.push_back(data);
        break;
    case FILE_TYPE: fileItems.push_back(data);
        break;
    }
}

void ItemInfo::normalize(void)
{
    for(int i =  1; i < fileItems.count(); ++i)
        items.push_back(fileItems[i]);
}

ItemInfo::Data& ItemInfo::operator [] (int index)
{
    return items[index];
}

void ItemInfo::setAccountOwner(const QString& name)
{
 accountOwner = name;
}

QString ItemInfo::getAccountOwner(void) const
{
  return accountOwner;
}

void ItemInfo::setDataOwner(const QString& name, int index, int type)
{
    switch(type)
    {
    case FOLDER_TYPE: items[index].dataOwner = name;
        break;
    case FILE_TYPE: fileItems[index].dataOwner = name;
        break;
    }
}


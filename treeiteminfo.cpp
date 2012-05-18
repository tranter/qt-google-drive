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

void TreeItemInfo::push_back(TreeItemInfo::Data& data, int type)
{
    switch(type)
    {
    case FOLDER_TYPE: items.push_back(data);
        break;
    case FILE_TYPE: fileItems.push_back(data);
        break;
    }
}

void TreeItemInfo::normalize(void)
{
    for(int i =  1; i < fileItems.count(); ++i)
        items.push_back(fileItems[i]);
}

TreeItemInfo::Data& TreeItemInfo::operator [] (int index)
{
    return items[index];
}

void TreeItemInfo::setAccountOwner(const QString& name)
{
 accountOwner = name;
}

QString TreeItemInfo::getAccountOwner(void) const
{
  return accountOwner;
}

void TreeItemInfo::setDataOwner(const QString& name, int index, int type)
{
    switch(type)
    {
    case FOLDER_TYPE: items[index].dataOwner = name;
        break;
    case FILE_TYPE: fileItems[index].dataOwner = name;
        break;
    }
}

//void TreeItemInfo::showFiles(void)
//{
//    for(int i = 0; i < fileItems.count(); ++i)
//        qDebug() << "file: " << fileItems[i].name;
//}


#include "treeitem.h"
#include "treemodel.h"
#include <QDebug>
#include <QIcon>

TreeModel::TreeModel(QList<QVariant> rootData, TreeItemInfo* treeItemsInfo, QObject *parent) :
    QAbstractItemModel(parent),
    itemInfo(treeItemsInfo),
    columnsTotal(rootData.count())
{
    init(rootData);
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

int TreeModel::init(QList<QVariant> rootData)
{
    rootItem = new TreeItem(rootData);
    setupModelData(rootItem);
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    QVariant columnData = item->data(index.column());

    if (role == Qt::DecorationRole)
    {
        if(columnData.toString()[0] == INFO_TOKEN) return  "";
        return QIcon(itemInfo->getItems()[getCurrentModelItemIndex(static_cast<TreeItem*>(index.internalPointer()))].iconPath);
    }

    if (role != Qt::DisplayRole) return QVariant();

    if(columnData.toString()[0] == INFO_TOKEN)
    {
        QString infoStr(columnData.toString());
        infoStr.remove(0,1);
        return  infoStr.toAscii();
    }

    return columnData;
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);

    else
        return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

void TreeModel::setupModelData(TreeItem *parent)
{
    buildTree(QString(INFO_TOKEN) + ROOT_FOLDER, parent);
}

void TreeModel::buildTree(const QString& searchStr, TreeItem *parent)
{
    QList< QList<QVariant> > columnData;
    QList<QVariant> selfs;
    QList<int> indexes;

    int count = 0;

    for (int i = itemInfo->getItems().count() -1; i >=0 ; --i)
    {
        if(itemInfo->getItems()[i].parent.toString() == searchStr)
        {
            QList<QVariant> column;

            column.push_back(itemInfo->getItems()[i].name);

            /*
             add other columns here if necessary
             (info columns - i.e. columns which contain additional info about item) (example: column.push_back(OTHER_INFO_COLUMN))

             NOTE:
                  also you must add columns into model before using this feature (method: void DriveEngine::setModel(void))
                  see comment in driveengine.cpp (method: void DriveEngine::setModel(void))
            */

            columnData.push_back(column);
            selfs.push_back(itemInfo->getItems()[i].self);
            indexes.push_back(i);
            ++count;
        }
    }

    if(count > 0)
    {
        fillTree(columnData, parent, indexes);

        for (int i = count - 1; i >= 0; --i)
        {
            buildTree(selfs[i].toString(), parent->child(i));
        }
    }
}

void TreeModel::fillTree(QList< QList<QVariant> > columnData, TreeItem *parent, QList<int> indexes)
{
    for (int i = 0; i < columnData.count(); ++i)
    {
        TreeItem* item = new TreeItem(columnData[i], parent);
        parent->appendChild(item);
        itemInfo->setItemPointer(indexes[i], item);
    }
}

int TreeModel::getCurrentModelItemIndex(TreeItem *item) const
{
    int currentModelIndex = 0;

    for(int i = 1; i < itemInfo->getItems().count(); ++i)
    {
        if(itemInfo->getItems()[i].item == item)
        {
            currentModelIndex = i;
            break;
        }
    }

    return currentModelIndex;
}

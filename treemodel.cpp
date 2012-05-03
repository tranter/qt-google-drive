#include "treeitem.h"
#include "treemodel.h"
#include <QDebug>

TreeModel::TreeModel(const QString &data, QList<QVariant> rootData, TreeItemInfo* itensInfo, QObject *parent)
    : QAbstractItemModel(parent)
{
    init(data, rootData, itensInfo);
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

int TreeModel::init(const QString &data, QList<QVariant> rootData, TreeItemInfo* itensInfo)
{
    rootItem = new TreeItem(rootData);
    setupModelData(data, rootItem, itensInfo);
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
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(index.column());
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

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
const
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

void TreeModel::setupModelData(const QString& data, TreeItem *parent, TreeItemInfo* itemsInfo)
{
    buildTree(ROOT_FOLDER, parent, itemsInfo);
}

void TreeModel::buildTree(const QString& searchStr, TreeItem *parent, TreeItemInfo* itemsInfo)
{
    QList< QList<QVariant> > columnData;
    QList<QVariant> selfs;
    int count = 0;

    for (int i = 0; i < itemsInfo->items.count(); ++i)
    {
        if(itemsInfo->items[i].parent.toString() == searchStr)
        {
            QList<QVariant> column;

            column.push_back(itemsInfo->items[i].name);
            columnData.push_back(column);
            selfs.push_back(itemsInfo->items[i].self);
            ++count;
        }
    }

    if(count > 0)
    {
        fillTree(columnData, parent);

        for (int i = 0; i < count; ++i)
        {
            buildTree(selfs[i].toString(), parent->child(i), itemsInfo);
        }
    }
}

void TreeModel::fillTree(QList< QList<QVariant> > columnData, TreeItem *parent)
{
    for (int i = 0; i < columnData.count(); ++i)
    {
        parent->appendChild(new TreeItem(columnData[i], parent));
    }
}

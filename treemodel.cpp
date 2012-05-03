#include "treeitem.h"
#include "treemodel.h"
#include <QDebug>

TreeModel::TreeModel(const QString &data, QList<QVariant> rootData, QList<TreeItemInfo::Data> items, QObject *parent)
    : QAbstractItemModel(parent)
{   
    init(data, rootData, items);
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

int TreeModel::init(const QString &data, QList<QVariant> rootData, QList<TreeItemInfo::Data> items)
{
    rootItem = new TreeItem(rootData);
    setupModelData(data, rootItem, items);
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

void TreeModel::setupModelData(const QString &data, TreeItem *parent, QList<TreeItemInfo::Data> items)
{
//    TreeItem *curNode = parent;
//    QString str = data;
//    QList<QVariant> columnData;

//    columnData.append(str + "1"); columnData.append(str + "2");
//    curNode->appendChild(new TreeItem(columnData, curNode));
//    //qDebug() << "------------>" << columnData;

//    curNode = parent->child(0);
//    columnData[0] = str + "3"; columnData[1] = str + "4";
//    curNode->appendChild(new TreeItem(columnData,  curNode));
//    //qDebug() << "------------>" << columnData;

//    curNode = curNode->child(0);
//    columnData[0] = str + "5"; columnData[1] = str + "6";
//    curNode->appendChild(new TreeItem(columnData,  curNode));
//    //qDebug() << "------------>" << columnData;

//    columnData[0] = str + "7"; columnData[1] = str + "8";
//    parent->appendChild(new TreeItem(columnData, parent));
//    //qDebug() << "------------>" << columnData;

//    //qDebug() << "------------>" << parent->child(0)->data(0);


}

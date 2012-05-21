#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "treeiteminfo.h"
#include "Def.h"

class TreeItem;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    TreeModel(QList<QVariant> rootData, TreeItemInfo* treeItemsInfo, QObject *parent = 0);
    ~TreeModel();

public:
    QVariant data(const QModelIndex &modelIndex, int role) const;
    Qt::ItemFlags flags(const QModelIndex &modelIndex) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &modelIndex) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    void setupModelData(TreeItem *parent);
    int init(QList<QVariant> rootData);
    int columnCount();
    //void sort ( int column, Qt::SortOrder order = Qt::AscendingOrder );

private:
    void fillTree(QList< QList<QVariant> > columnData, TreeItem *parent, QList<int> indexes);
    void buildTree(const QString& searchStr, TreeItem *parent);
    int getCurrentModelItemIndex(TreeItem *item) const;

private:   
    TreeItem *rootItem;
    TreeItemInfo* itemInfo;
    int columnsTotal;
};


#endif

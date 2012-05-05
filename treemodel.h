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
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    void setupModelData(TreeItem *parent);
    int init(QList<QVariant> rootData);

private:
    void fillTree(QList< QList<QVariant> > columnData, TreeItem *parent, QList<int> indexes);
    void buildTree(const QString& searchStr, TreeItem *parent);
    //bool allowDraw(const QModelIndex &index) const;
    int getCurrentModelItemIndex(TreeItem *item) const;

private:   
    TreeItem *rootItem;
    TreeItemInfo* itemInfo;
    int columnsTotal;
    QList<QVariant> rootData1;
};


#endif

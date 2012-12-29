#ifndef ITEMINFO_H
#define ITEMINFO_H

#include <QVariant>
#include <QTreeWidgetItem>
#include "share/defs.h"
#include "share/debug.h"

class ItemInfo
{
public:
    ItemInfo();

    struct Data
    {
        enum ESortOrder
        {
            EType = 0,
            EName,
            ETypeName
        };

        Data() : sortOrder(EName) {}

        QTreeWidgetItem* pointer;
        QString type;
        QString dataOwner;
        QString fileType;
        QString fileSize;
        QString fileUpdated;
        QString name;
        QString self;
        QString parent;
        QString iconPath;
        QString downloadLink;
        QString uploadLink;

        bool operator < (const Data &other) const { return sort(other); }
        void setSortOrder(ESortOrder so) { sortOrder = so; }

    private:
        bool sort(const Data &other) const
        {
            switch(sortOrder)
            {
            case EType: return sortByType(other);
            case EName: case ETypeName: return sortByName(other);
            }
        }

        bool sortByName(const Data &other) const { return name.toLower() < other.name.toLower(); }
        bool sortByType(const Data &other) const { return type < other.type; }

    private:
        ESortOrder sortOrder;
    };

public:
    void setFileSize(const QString &size, int index);
    void pushBack(Data &data);
    const QList<Data> &getItems() const;
    void setAccountOwner(const QString &name);
    QString getAccountOwner(void) const;
    void setDataOwner(const QString &name, int index);
    void sort(QList<ItemInfo::Data> &sortItems, Data::ESortOrder itemSortOrder, Qt::SortOrder sortOrder);

private:
    void setItemsSortOrder(QList<Data> &sortItems, Data::ESortOrder itemSortOrder);
    void sortByTypeName(QList<ItemInfo::Data> &sortItems, Qt::SortOrder sortOrder);

private:
    QList<Data> items;
    QString accountOwner;
};

#endif // TREEITEMINFO_H

#ifndef ITEMINFO_H
#define ITEMINFO_H

#include <QVariant>
#include <QTreeWidgetItem>
#include <QDateTime>
#include "share/defs.h"
#include "share/debug.h"

class Items
{
public:
    Items();

    struct Data
    {
        enum ESortOrder
        {
            ETypeName = 0,
            EDateTime,
            ESize
        };

        Data() : sortOrder(ETypeName) {}

        QTreeWidgetItem* pointer;
        QString type;
        QString fileType;
        QString dataOwner;
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
            case ETypeName: return sortByName(other);
            case EDateTime: return sortByDateTime(other);
            case ESize: return sortBySize(other);
            }
        }

        bool sortByName(const Data &other) const { return name.toLower() < other.name.toLower(); }
        bool sortByDateTime(const Data &other) const {DEBUG << fileUpdated;  return QDateTime::fromString(fileUpdated) < QDateTime::fromString(other.fileUpdated); }
        bool sortBySize(const Data &other) const { return fileSize.toInt() < other.fileSize.toInt(); }

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
    void sort(QList<Items::Data> &sortItems, Data::ESortOrder itemSortOrder, Qt::SortOrder sortOrder);

private:
    void setItemsSortOrder(QList<Data> &sortItems, Data::ESortOrder itemSortOrder);
    void sortByTypeName(QList<Items::Data> &sortItems, Qt::SortOrder sortOrder);

private:
    QList<Data> items;
    QString accountOwner;
};

#endif // TREEITEMINFO_H

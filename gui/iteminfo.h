#ifndef TREEITEMINFO_H
#define TREEITEMINFO_H

#include <QVariant>
#include <QTreeWidgetItem>
#include "share/defs.h"

class ItemInfo
{
public:
    ItemInfo();

    struct Data
    {
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

        bool operator < (const Data &s) const
        {
            return name < s.name;
        }
    };

public:

    void setFileSize(const QString &size, int index);
    void push_back(Data &data);
    const QList<Data>& getItems(void) const;
    void setAccountOwner(const QString &name);
    QString getAccountOwner(void) const;
    void setDataOwner(const QString &name, int index);

private:
    QList<Data> fileItems;
    QString accountOwner;
};

#endif // TREEITEMINFO_H

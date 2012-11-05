#ifndef TREEITEMINFO_H
#define TREEITEMINFO_H

#include <QVariant>
#include <QTreeWidgetItem>
#include "share/Def.h"

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
    };

public:
    Data& operator [] (int index);

public:
    void setPointer(int index, QTreeWidgetItem* pointer);
    void setFileSize(const QString& size, int index);
    void push_back(Data& data, int type);
    const QList<Data>& getItems(void) const;
    const QList<Data>& getFileItems(void) const;
    void normalize(void);
    void setAccountOwner(const QString& name);
    QString getAccountOwner(void) const;
    void setDataOwner(const QString& name, int index, int type);

private:
    QList<Data> items;
    QList<Data> fileItems;
    QString accountOwner;
};

#endif // TREEITEMINFO_H

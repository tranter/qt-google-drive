#ifndef FOLDERSMANAGER_H
#define FOLDERSMANAGER_H

#include "contentmanager.h"

class FoldersManager : public ContentManager
{
    Q_OBJECT
public:
    explicit FoldersManager(QObject *parent = 0);

public:
    //void insertTreeItemFolder(const QString &name, QTreeWidget *parent);
    void createFolder(const QString &folderUrl, const QString &name);

private:
    //void buildTree(const QString& searchStr, QTreeWidgetItem* parent);
    //void fillTree(QList<QString> names, QList<QString> iconPathes, QTreeWidgetItem* parent, QList<int> indexes);
    virtual void show(void);

private:
    QList<ItemInfo::Data> folderItems;
};

#endif // FOLDERSMANAGER_H

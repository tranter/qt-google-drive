#ifndef FOLDERSMANAGER_H
#define FOLDERSMANAGER_H

#include "contentmanager.h"

class FoldersManager : public ContentManager
{
    Q_OBJECT
public:
    explicit FoldersManager(QObject *parent = 0);

public:
    void createFolder(const QString &folderUrl, const QString &name);

private:
    void show(void);

private:
    QList<Items::Data> folderItems;
};

#endif // FOLDERSMANAGER_H

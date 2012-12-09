#ifndef ADDITIONALFOLDESMANAGER_H
#define ADDITIONALFOLDESMANAGER_H

#include "network/filesmanager.h"

class AdditionalFoldersManager : public FilesManager
{
    Q_OBJECT
public:
    explicit AdditionalFoldersManager(QObject *parent = 0);
    virtual ~AdditionalFoldersManager();

//public:
//    void create(const QString& name, QString iconName = "");

//private:
//    QList<QTreeWidgetItem*> aFolders;
};

#endif // ADDITIONALFOLDESMANAGER_H

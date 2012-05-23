#ifndef ADDITIONALFOLDESMANAGER_H
#define ADDITIONALFOLDESMANAGER_H

#include "filesmanager.h"

class AdditionalFoldersManager : public FilesManager//ContentManager
{
    Q_OBJECT
public:
    explicit AdditionalFoldersManager(QObject *parent = 0);
    virtual ~AdditionalFoldersManager();

public:
    void create(const QString& name, QString iconName = "");

private:
    QList<QTreeWidgetItem*> additionalFolders;
};

#endif // ADDITIONALFOLDESMANAGER_H

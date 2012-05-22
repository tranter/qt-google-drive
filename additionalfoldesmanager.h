#ifndef ADDITIONALFOLDESMANAGER_H
#define ADDITIONALFOLDESMANAGER_H

//#include "contentmanager.h"
#include "filesmanager.h"

class AdditionalFoldersManager : public FilesManager//ContentManager
{
    Q_OBJECT
public:
    explicit AdditionalFoldersManager(QObject *parent = 0);
    virtual ~AdditionalFoldersManager();

public:
    void create(const QString& name, QString iconName = "");
    void clear(void);

private:
    //virtual void show(void);
    QList<QTreeWidgetItem*> additionalFolders;
};

#endif // ADDITIONALFOLDESMANAGER_H

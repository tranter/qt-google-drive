#ifndef COMPUTERCONTENTMANAGER_H
#define COMPUTERCONTENTMANAGER_H

#include <QObject>
#include "contentmanager.h"

class ComputerContentManager : public QObject, public ContentManager
{
   Q_OBJECT
public:
    explicit ComputerContentManager(QTreeWidget *p, int pn, QObject *parent = 0);
    virtual ~ComputerContentManager();

public:
    virtual QString getParentFolder() const;
    virtual QString back();

protected:
     virtual void updateItemsState();

protected:
    void show();
};



#endif // COMPUTERCONTENTMANAGER_H

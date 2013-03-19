#ifndef COMPUTERCONTENTMANAGER_H
#define COMPUTERCONTENTMANAGER_H

#include <QObject>

class ComputerContentManager : public QObject
{
   Q_OBJECT
public:
    explicit ComputerContentManager(QObject *parent = 0);
    virtual ~ComputerContentManager();

public:
    QString back(void);

protected:
    void show(void);
    void updateItemsState(void);    
};



#endif // COMPUTERCONTENTMANAGER_H

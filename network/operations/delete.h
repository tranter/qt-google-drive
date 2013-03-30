#ifndef DELETE_H
#define DELETE_H

#include "network/networkmanager.h"
#include "operation.h"
#include "gui/items.h"

class Delete : public NetworkManager, public Operation
{
    Q_OBJECT
public:
    Delete(QObject *parent = 0);

signals:
    void itemDeleted(Items::Data &sourceData);

public:
    void item(const Items::Data &source, bool update);
    void items(const QList<Items::Data> &sources);

protected slots:
    void slotReplyFinished(QNetworkReply*);

private:
    bool updateItemAfterDelete;
};

#endif // DELETE_H

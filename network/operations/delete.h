#ifndef DELETE_H
#define DELETE_H

#include "network/networkmanager.h"
#include "network/queries.h"
#include "gui/items.h"

class Delete : public NetworkManager
{
    Q_OBJECT
public:
    Delete(QObject *parent = 0);
    void item(const Items::Data &source);

protected slots:
    void slotReplyFinished(QNetworkReply*);

private:
     Queries queries;
};

#endif // DELETE_H

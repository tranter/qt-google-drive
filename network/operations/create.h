#ifndef CREATE_H
#define CREATE_H

#include "network/networkmanager.h"
#include "network/queries.h"
#include "gui/items.h"

class Create : public NetworkManager
{
    Q_OBJECT
public:
    Create(QObject *parent = 0);

protected slots:
    void slotPostFinished(QNetworkReply* reply);

public:
    void folder(const QString &name, const QString &parentFolderUrl);

private:
    Queries queries;
};

#endif // CREATE_H

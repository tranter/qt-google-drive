#ifndef COPY_H
#define COPY_H

#include "network/networkmanager.h"
#include "network/queries.h"
#include "gui/items.h"

class Copy : public NetworkManager
{
    Q_OBJECT
public:
    explicit Copy(QObject *parent = 0);

protected slots:
    void slotPostFinished(QNetworkReply* reply);

public:
    void file(const Items::Data &source, const QString &destFolderUrl);

private:
     Queries queries;
};

#endif // COPY_H

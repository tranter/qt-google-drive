#ifndef RENAME_H
#define RENAME_H

#include "network/networkmanager.h"
#include "network/queries.h"
#include "gui/items.h"

class Rename : public NetworkManager
{
    Q_OBJECT
public:
    Rename(QObject *parent = 0);

protected slots:
    void slotPutFinished(void);

public:
    void file(const Items::Data &source, const QString &newName);

private:
    Queries queries;
};

#endif // RENAME_H

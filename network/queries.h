#ifndef QUERIES_H
#define QUERIES_H

#include "gui/accountinfo.h"
#include "share/singleton.h"

class Queries
{
public:
    Queries();

public:
    void setAccountInfo(void);
    AccountInfo *accountInfoPointer(void) const;

private:
   QScopedPointer <AccountInfo> accountInfo;
};

typedef TSingleton<Queries> SQueries;

#endif // QUERIES_H

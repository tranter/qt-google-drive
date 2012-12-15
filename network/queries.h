#ifndef QUERIES_H
#define QUERIES_H

#include "gui/accountinfo.h"

class Queries
{
public:
    Queries();

public:
    void getAccountInfo(void);

private:
   QScopedPointer <AccountInfo> userInfo;
};

#endif // QUERIES_H

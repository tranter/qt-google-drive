#ifndef OPERATION_H
#define OPERATION_H

#include "network/queries.h"
#include "gui/items.h"
#include "share/enums.h"

class Operation
{
public:
    Operation(EOperations oId);

public:   
    virtual void file(const Items::Data &source, const QString &destFolderUrl){}
    virtual void files(const QList<Items::Data> &sources, const QString &destFolderUrl){}
    virtual void item(const Items::Data &source, bool update){}
    virtual void items(const QList<Items::Data> &sources){}

public:
    void updatePanelContent(bool opposite);
    EOperations getOperationId() const;

protected:
    Queries queries;
    Items::Data sourceData;
    QList<Items::Data> sourcesData;
    EOperations operationId;
};

#endif // OPERATION_H

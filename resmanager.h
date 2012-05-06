#ifndef RESMANAGER_H
#define RESMANAGER_H

#include <QMap>
#include "Def.h"

class ResManager
{
public:
    ResManager();

public:
    QString getResPath(const QString& type);

private:
    void setResources();

private:
    QMap<QString, QString> resources;

};

#endif // RESMANAGER_H

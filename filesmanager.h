#ifndef FILESMANAGER_H
#define FILESMANAGER_H

#include "contentmanager.h"

class FilesManager : public ContentManager //public NetworkManager
{
    Q_OBJECT
public:

    enum ECustomSort
    {
      EByDate = 0,
      EBySize
    };

    explicit FilesManager(QObject *parent = 0);

public:
    void get(const QString& url);
    void sort(int column, Qt::SortOrder order);

private:
    virtual void show(void);

private:
    bool firstRequest;
};

#endif // FILESMANAGER_H

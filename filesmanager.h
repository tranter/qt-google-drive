#ifndef FILESMANAGER_H
#define FILESMANAGER_H

#include "contentmanager.h"

class FilesManager : public ContentManager
{
    Q_OBJECT
public:

    enum ECustomSort
    {
      EByDate = 0,
      EBySize
    };

    explicit FilesManager(QObject *parent = 0);
    virtual ~FilesManager();

public:
    //void clear(void);
    void sort(int column, Qt::SortOrder order);

private:
    virtual void show(void);
};

#endif // FILESMANAGER_H

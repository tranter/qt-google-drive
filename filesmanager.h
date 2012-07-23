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

signals:
    void signalFirstPanelIsLoaded();

public:
    QTreeWidget* getPanel(void) const;
    void setPanel(QTreeWidget *p);
    void sort(int column, Qt::SortOrder order);

private:
    virtual void show(void);

private:
    QTreeWidget *panel;
};

#endif // FILESMANAGER_H

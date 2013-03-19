#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H

#include <QTreeWidget>
#include <QTreeWidgetItem>

class ContentManager
{
public:
    ContentManager();
    explicit ContentManager(QTreeWidget *p, int pn);

    virtual ~ContentManager();

public:
    virtual QString getParentFolder(void) const = 0;
    virtual QString back(void) = 0;

protected:
     virtual void updateItemsState(void) = 0;

public:
    virtual void clear(void);
    virtual QTreeWidget* getPanel(void) const;
    virtual void setPanel(QTreeWidget *p, int pn);

protected:
    virtual void show(void);
    virtual QString getDate(const QString &date);
    virtual QString getSize(const QString &size);

protected:
    void sectionClicked();

protected:
    QList<QTreeWidgetItem*> treeWidgetItems;
    QTreeWidget *panel;
    bool isRoot;
    int panelNum;
};

#endif // CONTENTMANAGER_H

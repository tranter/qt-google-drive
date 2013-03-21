#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QLabel>
#include "gui/controls/combobox.h"

class FilePanel;

class ContentManager
{
public:
    explicit ContentManager(FilePanel *fp);
    virtual ~ContentManager();

public:
    virtual void get(const QString &resourcePointer) = 0;
    virtual QString getParentFolder(void) const = 0;
    virtual QString back(void) = 0;
    virtual void update() = 0;
    virtual void accountsComboBoxItemActivated(const QString &text) = 0;
    virtual void showFilesOnPanel(QTreeWidgetItem *item) = 0;

protected:
     virtual void updateItemsState(void) = 0;
     virtual void fillAcountsComboBox(QMap<QString, QString> accountsMap, const QString &currentAccount) = 0;

public:
    virtual void clear(void);
    virtual QTreeWidget* getPanel(void) const;
    virtual bool hasItemParentSign(QTreeWidgetItem *item) const;

protected:
    virtual void show(void);
    virtual QString getDate(const QString &date);
    virtual QString getSize(const QString &size);

protected:
    void sectionClicked();

protected:
    QList<QTreeWidgetItem*> treeWidgetItems;
    QTreeWidget *panel;
    int panelNum;
    QLabel *pathLabel;
    ComboBox *accountsComboBox;
    bool isRoot;
};

#endif // CONTENTMANAGER_H

#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QLabel>
#include <QDateTime>
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
    virtual void update() = 0;
    virtual void accountsComboBoxItemActivated(const QString &text) = 0;
    virtual void showFilesOnPanel(QTreeWidgetItem *item) = 0;

protected:
    virtual void updateItemsState(void) = 0;
    virtual void fillComboBox(QMap<QString, QString> accountsMap, const QString &currentAccount) = 0;
    virtual QString back(void) = 0;
    virtual bool isRootFolder() = 0;

public:
    virtual void clear(void);
    virtual QTreeWidget* getPanel(void) const;
    virtual bool hasItemParentSign(QTreeWidgetItem *item) const;

protected:
    virtual void show(void);
    virtual QString getDate(QDateTime &fileDateTime);
    virtual QString getSize(qint64);

protected:
    void sectionClicked();

protected:
    QList<QTreeWidgetItem*> treeWidgetItems;
    QTreeWidget *panel;
    int panelNum;
    QLabel *pathLabel;
    ComboBox *drivesComboBox;
    bool isRoot;
};

#endif // CONTENTMANAGER_H

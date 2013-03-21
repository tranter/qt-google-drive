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
    virtual QString getParentFolder() const = 0;
    virtual void update() = 0;
    virtual void accountsComboBoxItemActivated(const QString &text) = 0;
    virtual void showFilesOnPanel(QTreeWidgetItem *item) = 0;

protected:
    virtual void updateItemsState() = 0;
    virtual void fillComboBox(QMap<QString, QString> accountsMap, const QString &currentAccount) = 0; 
    virtual bool isRoot() = 0;

public:
    virtual void clear(void);
    virtual QTreeWidget* getPanel() const;
    virtual bool hasItemParentSign(QTreeWidgetItem *item) const;

protected:
    virtual void show(void);
    virtual QString getDate(QDateTime &fileDateTime);
    virtual QString getSize(qint64);
    virtual QString back();

protected:
    void sectionClicked();
    void addPath(const QString &path);

protected:
    QList<QTreeWidgetItem*> treeWidgetItems;
    QTreeWidget *panel;
    int panelNum;
    QLabel *pathLabel;
    ComboBox *drivesComboBox;
    QStringList pathes;
};

#endif // CONTENTMANAGER_H

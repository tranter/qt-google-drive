#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QLabel>
#include <QDateTime>
#include <QDateTime>
#include "gui/controls/combobox.h"

typedef QMap< QString, QMap<QString, QIcon> > ComboBoxItem;

class FilePanel;

class ContentManager
{
public:
    explicit ContentManager(FilePanel *fp);
    virtual ~ContentManager();

public:
    virtual void get(const QString &resourcePointer) = 0;
    virtual void update() = 0;
    virtual void accountsComboBoxItemActivated(const QString &text) = 0;
    virtual void showFilesOnPanel(QTreeWidgetItem *item) = 0;
    virtual QString parentFolder() = 0;

protected:
    virtual void updateItemsState(QByteArray &values) = 0;
    virtual bool isRoot() = 0;   

public:
    virtual void clear(void);
    virtual QTreeWidget* getPanel() const;
    virtual bool hasItemParentSign(QTreeWidgetItem *item) const;
    virtual void headerSectionClicked(int logicalIndex);

protected:
    virtual void show(void);
    virtual QString getDate(QDateTime &fileDateTime);
    virtual QString getSize(qint64);
    virtual QString back();
    virtual void fillComboBox(ComboBoxItem drivesMap, const QString &currentDrive);

protected:  
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

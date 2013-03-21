#ifndef COMPUTERCONTENTMANAGER_H
#define COMPUTERCONTENTMANAGER_H

#include <QObject>
#include <QFileInfo>
#include <QDir>
#include "contentmanager.h"
#include "gui/tools/fileiconprovider.h"

class ComputerContentManager : public QObject, public ContentManager
{
   Q_OBJECT
public:
    explicit ComputerContentManager(FilePanel *fp, QObject *parent = 0);
    ~ComputerContentManager();

public:
    void get(const QString &resourcePointer);
    QString getParentFolder() const;
    void update();
    void accountsComboBoxItemActivated(const QString &text);
    QString back();
    void showFilesOnPanel(QTreeWidgetItem *item);

protected:
     void updateItemsState();
     void fillAcountsComboBox(QMap<QString, QString> accountsMap, const QString &currentAccount);
     void addItem(QFileInfo &fileInfo);

protected:
    void show();

private:
     QFileIconProvider fileIconProvider;
     QDir dir;
};

#endif // COMPUTERCONTENTMANAGER_H

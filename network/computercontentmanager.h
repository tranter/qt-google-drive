#ifndef COMPUTERCONTENTMANAGER_H
#define COMPUTERCONTENTMANAGER_H

#include <QObject>
#include "contentmanager.h"

class ComputerContentManager : public QObject, public ContentManager
{
   Q_OBJECT
public:
    explicit ComputerContentManager(FilePanel *fp, QObject *parent = 0);
    ~ComputerContentManager();

public:
    void get(const QString &resourcePointer);
    QString getParentFolder() const;
    QString back();
    void update();
    void accountsComboBoxItemActivated(const QString &text);
    void showFilesOnPanel(QTreeWidgetItem *item);

protected:
     void updateItemsState();
     void fillAcountsComboBox(QMap<QString, QString> accountsMap, const QString &currentAccount);

protected:
    void show();
};

#endif // COMPUTERCONTENTMANAGER_H

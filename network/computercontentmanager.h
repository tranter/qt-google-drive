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
    void update();
    void accountsComboBoxItemActivated(const QString &text);
    void showFilesOnPanel(QTreeWidgetItem *item);
    virtual QString parentFolder();


protected:
    void updateItemsState(QByteArray &values);
    void addItem(QFileInfo &fileInfo);
    bool isRoot();

protected:
    void show();

private:
    QString makeRootPath(const QString &driveLetter);
    QString makeRootDrive(const QString &drive);
    void setSorting(int sortSection, Qt::SortOrder sortOrder);

private:
    QFileIconProvider fileIconProvider;
    QDir dir;
};

#endif // COMPUTERCONTENTMANAGER_H

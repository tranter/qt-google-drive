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
    QString getUpperLevelFolderURL(void) const;
    ItemInfo::Data getUpperLevelFolderInfo(void) const;
    QTreeWidget* getPanel(void) const;
    QString back(void);
    void setPanel(QTreeWidget *p);
    void sort(int column, Qt::SortOrder order);
    void deleteFile(const QString &url);
    void copyWebFile(const ItemInfo::Data &source, const QString &destFolder);
    void moveWebFile(const ItemInfo::Data &source, const QString &destFolder);
    void renameWebFile(const ItemInfo::Data &source, const QString &newName);
    void shareWebFile(const ItemInfo::Data &source);
    ItemInfo::Data getCurrentFileInfo(void);
    QStringList getPathesURLs(void) const;
    void setPathesURLs(const QStringList &pathesURLsStrList);

private:
    virtual void show(void);

private:
    void setItems(ItemInfo::Data::ESortOrder itemSortOrder = ItemInfo::Data::EType, Qt::SortOrder sortOrder = Qt::AscendingOrder);
    void addItem(const ItemInfo::Data &itemData);

private:
    QTreeWidget *panel;
    QStringList pathesURLs;
    ItemInfo::Data rootData;
    QList<ItemInfo::Data> normalizedItems;
    bool isRoot;
};

#endif // FILESMANAGER_H

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
    QString getUpLevelFolderLink(void) const;
    QTreeWidget* getPanel(void) const;
    QString back(void);
    void setPanel(QTreeWidget *p);
    void sort(int column, Qt::SortOrder order);
    void deleteFile(const QString &url);
    void copyWebFile(const ItemInfo::Data &source, const QString &destFolder);
    void moveWebFile(const ItemInfo::Data &source, const QString &destFolder);
    ItemInfo::Data getCurrentFileInfo(void);

private:
    virtual void show(void);

private:
    QTreeWidget *panel;
    QStringList pathLinks;
    QList<ItemInfo::Data> fileItems;
    bool isRoot;
};

#endif // FILESMANAGER_H

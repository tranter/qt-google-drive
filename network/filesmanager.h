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
    QString getCurrentFolderLink(void) const;
    QTreeWidget* getPanel(void) const;
    QString back(void);
    void setPanel(QTreeWidget *p);
    void sort(int column, Qt::SortOrder order);
    void deleteFile(const QString &url);
    void copyWebFile(const QString &source, const QString &dest);

private:
    virtual void show(void);

private:
    QTreeWidget *panel;
    QStringList links;
};

#endif // FILESMANAGER_H

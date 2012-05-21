#ifndef FOLDERSMANAGER_H
#define FOLDERSMANAGER_H

#include "contentmanager.h"

class FoldersManager : public ContentManager// NetworkManager
{
    Q_OBJECT
public:
    explicit FoldersManager(QObject *parent = 0);

public slots:
    virtual void slotError(QNetworkReply::NetworkError error);

signals:
    void signalAccessTokenRequired();

private:
    void buildTree(const QString& searchStr, QTreeWidgetItem* parent);
    void fillTree(QList<QString> names, QList<QString> iconPathes, QTreeWidgetItem* parent, QList<int> indexes);
    virtual void show(void);

private:
    QList<TreeItemInfo::Data> folderItems;
};

#endif // FOLDERSMANAGER_H

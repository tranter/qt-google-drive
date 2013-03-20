#ifndef WEBCONTENTMANAGER_H
#define WEBCONTENTMANAGER_H

#include "contentmanager.h"
#include "parsers/xmlparser.h"
#include "network/networkmanager.h"

class WebContentManager : public NetworkManager, public ContentManager
{
    Q_OBJECT
public:
    explicit WebContentManager(QTreeWidget *p, int pn, QObject *parent = 0);
    virtual ~WebContentManager();

signals:
    void signalFirstPanelIsLoaded();

private slots:
        virtual void slotReplyFinished(QNetworkReply* reply);
        virtual void slotSectionClicked(int logicalIndex);

public:
    QString getParentFolder() const;
    QString back();

protected:
    void updateItemsState();
    void show();

public:
    void get(const QString &url);
    Items::Data getParentFolderInfo() const;
    Items::Data getCurrentItem();
    QStringList getPathesURLs() const;
    void setPathesURLs(const QStringList &pathesURLsStrList);
    void getItemsDataByIndexes(QList<int> &indexes, QList<Items::Data> &folders, QList<Items::Data> &files);
    int getIndexByItemData(QTreeWidget *treeWidget, Items::Data &itemData) const;
    void cashIcons();

private:
    bool parseReply(const QString &str);
    void setItems(Items::Data::ESortOrder itemSortOrder, Qt::SortOrder sortOrder);
    void addItem(const Items::Data &itemData);

private:
    QScopedPointer<XMLParser> parser;
    QStringList pathesURLs;
    Items::Data parentData;
    QList<Items::Data> normalizedItems;
};

#endif // WEBCONTENTMANAGER_H

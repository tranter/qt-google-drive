#ifndef WEBCONTENTMANAGER_H
#define WEBCONTENTMANAGER_H

#include <QXmlSimpleReader>
#include "contentmanager.h"
#include "parsers/xmlparser.h"
#include "network/networkmanager.h"
#include "network/operations/operationsmanager.h"

class WebContentManager : public NetworkManager, public ContentManager
{
    Q_OBJECT
public:
    explicit WebContentManager(QObject *parent = 0);
    explicit WebContentManager(QTreeWidget *p, int pn, QObject *parent = 0);

    virtual ~WebContentManager();

signals:
    void signalFirstPanelIsLoaded();

private slots:
        virtual void slotReplyFinished(QNetworkReply* reply);
        virtual void slotSectionClicked(int logicalIndex);

public:
    QString getParentFolder(void) const;
    QString back(void);

protected:
    void updateItemsState(void);
    void show(void);

public:
    void get(const QString &url);
    XMLParser* getParser(void) const; 
    Items::Data getParentFolderInfo(void) const; 
    Items::Data getCurrentItem(void);
    QStringList getPathesURLs(void) const;
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

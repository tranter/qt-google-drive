#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H

#include <QXmlSimpleReader>
#include "parsers/xmlparser.h"
#include "network/networkmanager.h"
#include "network/operationsmanager.h"

class ContentManager : public NetworkManager
{
    Q_OBJECT
public:
    explicit ContentManager(QObject *parent = 0);
    virtual ~ContentManager();

public:
    virtual void get(const QString &url);
    XMLParser* getParser(void) const;
    void clear(void);

public slots:
    virtual void slotReplyFinished(QNetworkReply* reply);
    void slotResDownloaded(void);

protected:
    virtual bool parseReply(const QString &str);
    //virtual void show(void) = 0;





signals:
    void signalFirstPanelIsLoaded();

private slots:
        void slotSectionClicked(int logicalIndex);

public:
    QString getParentFolderUrl(void) const;
    Items::Data getUpperLevelFolderInfo(void) const;
    QTreeWidget* getPanel(void) const;
    QString back(void);
    void setPanel(QTreeWidget *p, int pn);
    void sort(int column, Qt::SortOrder order);
    Items::Data getCurrentFileInfo(void);
    QStringList getPathesURLs(void) const;
    void setPathesURLs(const QStringList &pathesURLsStrList);

private:
    virtual void show(void);

private:
    void setItems(Items::Data::ESortOrder itemSortOrder, Qt::SortOrder sortOrder);
    void addItem(const Items::Data &itemData);
    QString getDate(const QString &date);
    QString getSize(const QString &size);
    void updateItemsState(void);






protected:
    QScopedPointer<XMLParser> parser;
    QList<QTreeWidgetItem*> treeWidgetItems;

//private:
    QTreeWidget *panel;
    QStringList pathesURLs;
    Items::Data rootData;
    QList<Items::Data> normalizedItems;
    bool isRoot;
    int panelNum;
};



#endif // CONTENTMANAGER_H

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
    explicit ContentManager(QObject *parent);
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
    virtual void show(void) = 0;

protected:
    QScopedPointer<XMLParser> parser;
    QList<QTreeWidgetItem*> treeWidgetItems;
};



#endif // CONTENTMANAGER_H

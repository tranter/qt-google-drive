#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H

#include <QXmlSimpleReader>
#include "xmlparser.h"
#include "networkmanager.h"

class ContentManager : public NetworkManager
{
    Q_OBJECT
public:
    explicit ContentManager(int handleType, QObject *parent = 0);
    virtual ~ContentManager();

public:
    virtual void get(const QString& url);
    XMLParser* getParser(void) const;
    void clear(void);

public slots:
    virtual void slotReplyFinished(QNetworkReply* reply);
    void slotResDownloaded(int queryType);

protected:
    virtual bool parseReply(const QString& str);
    virtual void show(void) = 0;

protected:
    int type;
    XMLParser* parser;
    QList<QTreeWidgetItem*> items;
};



#endif // CONTENTMANAGER_H

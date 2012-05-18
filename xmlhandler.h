#ifndef XMLHANDLER_H
#define XMLHANDLER_H

#include <QObject>
#include <QXmlAttributes>
#include "treeiteminfo.h"
#include "Def.h"
#include "resmanager.h"
#include "commontools.h"

class XMLHandler : public QObject
{
    Q_OBJECT
public:

    enum ETags{
        ETitle = 0,
        ESize,
        EUpdated,
        EAuthor,
        EEntry,
        ETagsCount
    };

    XMLHandler(int type);
    ~XMLHandler();

public:
    bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &attribs);
    bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);
    bool characters(const QString &str);
    bool fatalError(const QXmlParseException &exception);

    TreeItemInfo* getTreeItemInfo(void) const;
    void setType(int type);
    bool resDownloadingNow(void) const;

public slots:
    void slotResDownloaded();

signals:
    void signalAllResDownloaded(int);

private:
    bool handleReply(const QString &qName, const QXmlAttributes &attribs, int queryType);
    void setDefaults();

private:
    int queryType;
    TreeItemInfo* itemInfo;
    TreeItemInfo::Data itemData;
    bool tags[ETagsCount];
    QList<ResManager*> resManagers;
    int resDownloadedCount;
    QLocale locale;
    bool isResDownloding;
};

#endif // XMLHANDLER_H

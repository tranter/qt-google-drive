#ifndef XMLHANDLER_H
#define XMLHANDLER_H

#include <QObject>
#include <QXmlAttributes>
#include "gui/iteminfo.h"
#include "share/Def.h"
#include "gui/resmanager.h"

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

    ItemInfo* getItemInfo(void) const;
    void setType(int type);
    bool resDownloadingNow(void) const;

public slots:
    void slotResDownloaded();

signals:
    void signalAllResDownloaded(int);

private:
    bool handleReply(const QString &qName, const QXmlAttributes &attribs);
    void handleAuthorTag(const QString &str, bool entryTag);
    void handleTitleTag(const QString &str);
    void handleSizeTag(const QString &str);
    void handleUpdatedTag(const QString &str);
    void setDefaults(void);
    void setTag(const QString& qName, bool state);
    void setItemType(QString &itemType);
    void saveResData(const QXmlAttributes &attribs);

private:
    QScopedPointer<ItemInfo> itemInfo;
    ItemInfo::Data itemData;
    QList<ResManager*> resManagers;
    QLocale locale;

    int queryType;
    int resDownloadedCount;
    bool isResDownloding;
    bool tags[ETagsCount];     
};

#endif // XMLHANDLER_H

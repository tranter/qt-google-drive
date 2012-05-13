#ifndef XMLHANDLER_H
#define XMLHANDLER_H

#include <QXmlAttributes>
#include "treeiteminfo.h"
#include "Def.h"
#include "resmanager.h"
#include "commontools.h"

class XMLHandler
{
public:

    enum ETags{
        ETitle = 0,
        ESize,
        EUpdated,
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

private:
    bool handleReply(const QString &qName, const QXmlAttributes &attribs, int queryType);

private:
    int queryType;
    TreeItemInfo* itemInfo;
    TreeItemInfo::Data itemData;
    bool tags[ETagsCount];
    ResManager resManager;
    QString infoToken;
    QLocale locale;
};

#endif // XMLHANDLER_H

#ifndef XMLHANDLER_H
#define XMLHANDLER_H

#include <QXmlAttributes>
#include "treeiteminfo.h"
#include "Def.h"
#include "resmanager.h"

class XMLHandler
{
public:
    XMLHandler(int type);
    ~XMLHandler();

public:
    bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &attribs);
    bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);
    bool characters(const QString &str);
    bool fatalError(const QXmlParseException &exception);

    TreeItemInfo* getTreeItemInfo(void) const;

private:
    void handleFolders(const QString &qName, const QXmlAttributes &attribs);

private:
    int queryType;
    TreeItemInfo* itemInfo;
    TreeItemInfo::Data itemData;
    bool isTitle;
    ResManager resManager;
    QString infoToken;

};

#endif // XMLHANDLER_H

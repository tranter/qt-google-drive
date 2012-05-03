#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QXmlDefaultHandler>
#include "treeiteminfo.h"


class XMLParser : public QXmlDefaultHandler
{
public:
    XMLParser();
    ~XMLParser();

public:
    bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &attribs);
    bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);
    bool characters(const QString &str);
    bool fatalError(const QXmlParseException &exception);
    TreeItemInfo* getTreeItemInfo(void) const;

private:
     TreeItemInfo* treeItemInfo;
     TreeItemInfo::Data itemData;
     bool isTitle;

};

#endif // XMLPARSER_H

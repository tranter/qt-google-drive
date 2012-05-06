#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QXmlDefaultHandler>
#include "xmlhandler.h"

class XMLParser : public QXmlDefaultHandler
{
public:
    XMLParser(int type);
    ~XMLParser();

public:
    bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &attribs);
    bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);
    bool characters(const QString &str);
    bool fatalError(const QXmlParseException &exception);
    XMLHandler* getXMLHandler(void) const;
    void setType(int type);

private:
    XMLHandler* xmlHandler;

};

#endif // XMLPARSER_H

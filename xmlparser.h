#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QXmlDefaultHandler>

class XMLParser : public QXmlDefaultHandler
{
public:
    XMLParser(const QString str);

public:
    bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &attribs);
    bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);
    bool characters(const QString &str);
    bool fatalError(const QXmlParseException &exception);

public:
    void SetCurrentAnalyzingName(const QString& str);


private:
     QString parseStr;
};

#endif // XMLPARSER_H

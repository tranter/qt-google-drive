#include "xmlparser.h"

XMLParser::XMLParser(const QString str):
    parseStr(str)
{
}

bool XMLParser::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &attribs)
{

    return true;
}

bool XMLParser::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{

    return true;
}

bool XMLParser::characters(const QString &str)
{
    return true;
}

bool XMLParser::fatalError(const QXmlParseException &exception)
{
    return true;
}

void XMLParser::SetCurrentAnalyzingName(const QString& str)
{
}

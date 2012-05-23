#include "xmlparser.h"

XMLParser::XMLParser(int type):
    xmlHandler(new XMLHandler(type))
{
}

XMLParser::~XMLParser()
{
    xmlHandler->deleteLater();
}

bool XMLParser::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &attribs)
{
    return xmlHandler->startElement(namespaceURI, localName, qName, attribs);
}

bool XMLParser::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
    return xmlHandler->endElement(namespaceURI, localName, qName);
}

bool XMLParser::characters(const QString &str)
{
    return xmlHandler->characters(str);
}

bool XMLParser::fatalError(const QXmlParseException &exception)
{
    return xmlHandler->fatalError(exception);
}

XMLHandler* XMLParser::getXMLHandler(void) const
{
    return xmlHandler;
}

void XMLParser::setType(int type)
{
    xmlHandler->setType(type);
}

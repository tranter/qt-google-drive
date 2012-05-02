#include "xmlparser.h"
#include <QDebug>

XMLParser::XMLParser(const QString str):
    parseStr(str)
{
}

bool XMLParser::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &attribs)
{
    qDebug() << "startElement qName=" << qName;
    return true;
}

bool XMLParser::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
    qDebug() << "endElement qName=" << qName << endl;
    return true;
}

bool XMLParser::characters(const QString &str)
{
    qDebug() << "characters=" << str;
    return true;
}

bool XMLParser::fatalError(const QXmlParseException &exception)
{
    qDebug() << "fatalError=" << exception.message();
    return true;
}

void XMLParser::SetCurrentAnalyzingName(const QString& str)
{
}

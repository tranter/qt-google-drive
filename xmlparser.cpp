#include "xmlparser.h"
#include <QDebug>

XMLParser::XMLParser(const QString str):
    parseStr(str)
{
}

bool XMLParser::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &attribs)
{
    if(qName == "entry")
    {
        qDebug() << "startElement localName = " << localName << " qName=" << qName << " attribs=" << attribs.value("href");
    }

    if(attribs.value("rel") == "http://schemas.google.com/docs/2007#parent")
    {
        qDebug() << "parent=" << attribs.value("href");
    }

    if(attribs.value("rel") == "self")
    {
        qDebug() << "self=" << attribs.value("href");
    }

    return true;
}

bool XMLParser::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
    if(qName == "entry")
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

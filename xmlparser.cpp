#include "xmlparser.h"
#include "treeiteminfo.h"
#include <QDebug>

XMLParser::XMLParser()
{
}

bool XMLParser::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &attribs)
{
    if(attribs.value("rel") == "http://schemas.google.com/docs/2007#parent")
    {
        itemData.item = NULL;
        itemData.name = attribs.value("title");
        itemData.parent = attribs.value("href");
    }

    if(attribs.value("rel") == "self")
    {
         itemData.self = attribs.value("href");
         treeItemInfo.items.push_back(itemData);
    }

    return true;
}

bool XMLParser::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
    //if(qName == "entry") treeItemInfo.showAll();
        //qDebug() << "endElement qName=" << qName << endl;
    return true;
}

bool XMLParser::characters(const QString &str)
{
    //qDebug() << "characters=" << str;
    return true;
}

bool XMLParser::fatalError(const QXmlParseException &exception)
{
    qDebug() << "fatalError=" << exception.message();
    return true;
}

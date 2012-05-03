#include "xmlparser.h"
#include "treeiteminfo.h"
#include <QDebug>

XMLParser::XMLParser():
    treeItemInfo(new TreeItemInfo),
    isTitle(false)
{  
}

XMLParser::~XMLParser()
{
 delete treeItemInfo;
}

bool XMLParser::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &attribs)
{
    if(qName == "title") isTitle = true;

    if(attribs.value("rel") == "http://schemas.google.com/docs/2007#parent")
    {
        itemData.item = NULL;
        itemData.parent = attribs.value("href");
        //qDebug() << "------------->parent=" << attribs.value("href");
    }

    if(attribs.value("rel") == "self")
    {
         itemData.self = attribs.value("href");
         treeItemInfo->items.push_back(itemData);
         //qDebug() << "------------->self=" << attribs.value("href");
    }

    return true;
}

bool XMLParser::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
    //if(qName == "entry")
        //qDebug() << "endElement qName=" << qName << endl;
    return true;
}

bool XMLParser::characters(const QString &str)
{
    if(isTitle) itemData.name = str;
    //qDebug() << "characters=" << str;
    isTitle = false;

    return true;
}

bool XMLParser::fatalError(const QXmlParseException &exception)
{
    qDebug() << "fatalError=" << exception.message();
    return true;
}

TreeItemInfo* XMLParser::getTreeItemInfo(void) const
{
  return treeItemInfo;
}

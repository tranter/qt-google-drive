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
    if(qName == TITLE_TAG) isTitle = true;

    if(HIERARCHY_ATTRIBUTE == PARENT_FOLDER)
    {
        itemData.item = NULL;
        itemData.parent = HIERARCHY_VALUE;
    }

    if(HIERARCHY_ATTRIBUTE == SELF_TAG)
    {
         itemData.self = HIERARCHY_VALUE;
         treeItemInfo->items.push_back(itemData);
    }

    return true;
}

bool XMLParser::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
    return true;
}

bool XMLParser::characters(const QString &str)
{
    if(isTitle) itemData.name = str;
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

#include "xmlparser.h"
#include "treeiteminfo.h"
#include <QDebug>

XMLParser::XMLParser(int type):
    queryType(type),
    itemInfo(new TreeItemInfo),
    isTitle(false)
{
}

XMLParser::~XMLParser()
{
    delete itemInfo;
}

bool XMLParser::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &attribs)
{
    switch(queryType)
    {
    case FOLDER_TYPE:
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
            itemInfo->items.push_back(itemData);
            itemData.type = FOLDER_TYPE_STR;
            itemData.iconPath = resManager.getResPath(FOLDER_TYPE_STR);
        }
    }
        break;

    case FILE_TYPE:
    {

    }
        break;
    }
    return true;
}

bool XMLParser::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
    return true;
}

bool XMLParser::characters(const QString &str)
{
    if(isTitle)
    {
        itemData.name = str;
    }

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
    return itemInfo;
}

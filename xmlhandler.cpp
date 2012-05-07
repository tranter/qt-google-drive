#include "xmlhandler.h"
#include <QDebug>

XMLHandler::XMLHandler(int type):
    queryType(type),
    itemInfo(new TreeItemInfo),
    isTitle(false),
    infoToken(QString(INFO_TOKEN))
{
}

XMLHandler::~XMLHandler()
{
    delete itemInfo;
}

bool XMLHandler::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &attribs)
{
    switch(queryType)
    {
    case FOLDER_TYPE: handleFolders(qName, attribs);
        break;

    case FILE_TYPE: handleFiles(qName, attribs);
        break;
    }

    return true;
}

bool XMLHandler::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
    return true;
}

bool XMLHandler::characters(const QString &str)
{
    if(isTitle)
    {
        itemData.name = str;
        qDebug() << "----------------> str" << str;
    }

    isTitle = false;

    return true;
}

TreeItemInfo* XMLHandler::getTreeItemInfo(void) const
{
    return itemInfo;
}

bool XMLHandler::fatalError(const QXmlParseException &exception)
{
    qDebug() << "XMLHandler::fatalError =" << exception.message();
    return true;
}

void XMLHandler::handleFolders(const QString &qName, const QXmlAttributes &attribs)
{
    if(qName == TITLE_TAG) isTitle = true;

    if(HIERARCHY_ATTRIBUTE == PARENT_FOLDER)
    {
        itemData.item = NULL;
        itemData.parent = infoToken + HIERARCHY_VALUE;
    }

    if(HIERARCHY_ATTRIBUTE == SELF_TAG)
    {
        itemData.self = infoToken + HIERARCHY_VALUE;
        itemData.type = FOLDER_TYPE_STR;
        itemData.iconPath = resManager.getResPath(FOLDER_TYPE_STR);
        itemInfo->push_back(itemData, TreeItemInfo::Efolder);
    }
}













//------------------------------------------------------------------------------

void XMLHandler::handleFiles(const QString &qName, const QXmlAttributes &attribs)
{
    if(qName == TITLE_TAG) isTitle = true;

    if(HIERARCHY_ATTRIBUTE == PARENT_FOLDER)
    {
        itemData.item = NULL;
        itemData.parent = infoToken + HIERARCHY_VALUE;

        qDebug() << "----------------> itemData.parent" << itemData.parent;
    }

    if(HIERARCHY_ATTRIBUTE == SELF_TAG)
    {
        itemData.self = infoToken + HIERARCHY_VALUE;
        itemData.type = FILE_TYPE_STR;
        itemData.iconPath = resManager.getResPath(FILE_TYPE_STR);

        qDebug() << "----------------> itemData.self" << itemData.self;

        itemInfo->push_back(itemData, TreeItemInfo::EFile);
    }
}

//------------------------------------------------------------------------------












void XMLHandler::setType(int type)
{
    queryType = type;
}


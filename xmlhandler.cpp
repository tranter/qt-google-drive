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
    return handleReply(qName, attribs, queryType);;
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
        qDebug() << "itemData.name = " << itemData.name;
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

bool XMLHandler::handleReply(const QString &qName, const QXmlAttributes &attribs, int queryType)
{
    QString resPath;
    TreeItemInfo::ETypes type;

    switch(queryType)
    {
    case FOLDER_TYPE:
    {
        resPath = FOLDER_TYPE_STR;
        type = TreeItemInfo::Efolder;
    }
        break;

    case FILE_TYPE:
    {
        resPath = FILE_TYPE_STR;
        type = TreeItemInfo::EFile;
    }
        break;
    }
    if(qName == CONTENT && resPath == FILE_TYPE_STR)
    {
        itemData.fileType = attribs.value(TYPE);
        itemData.downloadLink = attribs.value(SRC);
        qDebug() << "itemData.fileType = " << attribs.value(TYPE);
    }

    if(qName == TITLE_TAG) isTitle = true;

    if(HIERARCHY_ATTRIBUTE == PARENT_FOLDER)
    {
        itemData.item = NULL;
        itemData.parent = infoToken + HIERARCHY_VALUE;
    }

    if(HIERARCHY_ATTRIBUTE == SELF_TAG)
    {
        itemData.self = infoToken + HIERARCHY_VALUE;
        itemData.type = resPath;
        itemData.iconPath = resManager.getResPath(resPath);
        itemInfo->push_back(itemData, type);
    }

    return true;
}

void XMLHandler::setType(int type)
{
    queryType = type;
}


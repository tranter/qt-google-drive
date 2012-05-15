#include "xmlhandler.h"
#include <QDebug>

XMLHandler::XMLHandler(int type):
    queryType(type),
    itemInfo(new TreeItemInfo),
    infoToken(QString(INFO_TOKEN))
{
    for(int i = ETitle; i < ETagsCount; ++i) tags[i] = false;
}

XMLHandler::~XMLHandler()
{
    delete itemInfo;
}

bool XMLHandler::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &attribs)
{
    return handleReply(qName, attribs, queryType);
}

bool XMLHandler::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
    return true;
}

bool XMLHandler::characters(const QString &str)
{
    if(tags[ETitle])
    {
        itemData.name = str;
        tags[ETitle] = false;
    }

    if(tags[ESize])
    {
//        qlonglong size = str.toLongLong();
//        QString sizeStr = locale.toString(size);
          tags[ESize] = false;
//        itemInfo->setFileSize(infoToken + sizeStr + " bytes ", itemInfo->getFileItems().count() - 1);

        itemInfo->setFileSize(infoToken + CommonTools::getFormattedFileSize(str), itemInfo->getFileItems().count() - 1);
    }

    if(tags[EUpdated])
    {
        itemData.fileUpdated =  infoToken + CommonTools::convertDate(str);
        tags[EUpdated] = false;
    }

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
    TreeItemInfo::ETypes type;
    QString resPath;

    switch(queryType)
    {
    case FOLDER_TYPE:
    {
        resPath = FOLDER_TYPE_STR;
        type = TreeItemInfo::Efolder;
        itemData.fileSize = infoToken;
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
        itemData.fileType = FYLE_TYPE_ATTRIBUTE;
        itemData.downloadLink = FYLE_TYPE_SRC_ATTRIBUTE;
    }

    if(qName == FOLDER_TITLE_TAG) tags[ETitle]= true;
    if(qName == FILE_SIZE_TAG) tags[ESize] = true;
    if(qName == UPDATED_FILE_TAG) tags[EUpdated] = true;


    if(HIERARCHY_ATTRIBUTE == PARENT)
    {
        itemData.item = NULL;
        itemData.parent = infoToken + HIERARCHY_VALUE;
    }

    if(HIERARCHY_ATTRIBUTE == UPLOAD)
    {
        itemData.uploadLink = HIERARCHY_VALUE;
    }

    if(HIERARCHY_ATTRIBUTE == SELF)
    {
        itemData.self = infoToken + HIERARCHY_VALUE;
        itemData.type = resPath;
        itemData.iconPath = resManager.getResPath(resPath);
        itemInfo->push_back(itemData, type);
        setDefaults();
    }

    return true;
}

void XMLHandler::setType(int type)
{
    queryType = type;
}

void XMLHandler::setDefaults(void)
{
    itemData.item = NULL;
    itemData.fileSize = "";
    itemData.fileUpdated = "";
    itemData.parent = infoToken + ROOT;
}


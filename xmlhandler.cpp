#include "xmlhandler.h"
#include "mainwindow.h"
#include <QDebug>
//#include "qt_windows.h"

XMLHandler::XMLHandler(int type):
    queryType(type),
    itemInfo(new TreeItemInfo),
    //infoToken(QString(INFO_TOKEN)),
    resDownloadedCount(0),
    isResDownloding(false)
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
        tags[ESize] = false;
        itemInfo->setFileSize(/*infoToken + */CommonTools::getFormattedFileSize(str), itemInfo->getFileItems().count() - 1);
    }

    if(tags[EUpdated])
    {
        itemData.fileUpdated =  /*infoToken + */CommonTools::convertDate(str);
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
    QString itemType;

    switch(queryType)
    {
    case FOLDER_TYPE:
    {
        itemType = FOLDER_TYPE_STR;
        type = TreeItemInfo::Efolder;
        //itemData.fileSize = /*infoToken*/;
    }
        break;

    case FILE_TYPE:
    {
        itemType = FILE_TYPE_STR;
        type = TreeItemInfo::EFile;
    }
        break;
    }

    if(qName == CONTENT && itemType == FILE_TYPE_STR /* type == TreeItemInfo::EFile*/)
    {
        itemData.fileType = FYLE_TYPE_ATTRIBUTE_TAG;
        itemData.downloadLink = FYLE_TYPE_SRC_ATTRIBUTE_TAG;
    }

    if(qName == FOLDER_TITLE_TAG) tags[ETitle]= true;
    if(qName == FILE_SIZE_TAG) tags[ESize] = true;
    if(qName == UPDATED_FILE_TAG) tags[EUpdated] = true;

    if(HIERARCHY_ATTRIBUTE_TAG == PARENT_TAG)
    {
        itemData.item = NULL;
        itemData.parent = /*infoToken + */HIERARCHY_VALUE_TAG;
    }

    if(HIERARCHY_ATTRIBUTE_TAG == UPLOAD_TAG)
    {
        itemData.uploadLink = HIERARCHY_VALUE_TAG;
    }

    if(HIERARCHY_ATTRIBUTE_TAG == ICON_TAG)
    {
        if(!CommonTools::fileFromURLExists(HIERARCHY_VALUE_TAG))
        {
            isResDownloding = true;
            resManagers.push_back(new ResManager);
            resManagers.last()->cashRes(HIERARCHY_VALUE_TAG);
            connect(resManagers.last(), SIGNAL(signalResDownloaded()), this, SLOT(slotResDownloaded()));
        }

        itemData.iconPath = CommonTools::getFileNameFromURL(HIERARCHY_VALUE_TAG);
    }

    if(HIERARCHY_ATTRIBUTE_TAG == SELF_TAG)
    {
        itemData.self = /*infoToken + */HIERARCHY_VALUE_TAG;
        itemData.type = itemType;
        itemInfo->push_back(itemData, type);
        setDefaults();
    }

    return true;
}

void XMLHandler::slotResDownloaded()
{
    if(++resDownloadedCount == resManagers.count())
    {
        for(int i = 0; i < resManagers.count();++i) delete resManagers[i];
        resManagers.clear();
        resDownloadedCount = 0;
        emit signalAllResDownloaded(queryType);
    }
}

void XMLHandler::setType(int type)
{
    queryType = type;
}

void XMLHandler::setDefaults(void)
{
    itemData.item = NULL;
    itemData.fileSize = /*infoToken + */"---";
    itemData.fileUpdated = "";
    itemData.parent = /*infoToken + */ROOT_TAG;
}

bool XMLHandler::resDownloadingNow(void) const
{
    return isResDownloding;
}



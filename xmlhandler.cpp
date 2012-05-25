#include "xmlhandler.h"
#include "mainwindow.h"
#include <QDebug>

XMLHandler::XMLHandler(int type):
    queryType(type),
    itemInfo(new TreeItemInfo),
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
    if(tags[EAuthor] && !tags[EEntry])
    {
        itemInfo->setAccountOwner(str);
        tags[EAuthor] = false;
    }

    if(tags[EAuthor] && tags[EEntry])
    {
        int index;

        switch(queryType)
        {
        case FOLDER_TYPE: index = itemInfo->getItems().count() - 1;
            break;
        case FILE_TYPE: index = itemInfo->getFileItems().count()  -1;
            break;
        }

        QString author(str);

        if(itemInfo->getAccountOwner() == author) author = OWNER_ME;
        itemInfo->setDataOwner(author, index, queryType);

        tags[EAuthor] = false;
    }

    if(tags[ETitle])
    {
        itemData.name = str;
        tags[ETitle] = false;
    }

    if(tags[ESize])
    {
        itemInfo->setFileSize(CommonTools::getFormattedFileSize(str), itemInfo->getFileItems().count() - 1);
        tags[ESize] = false;
    }

    if(tags[EUpdated])
    {
        itemData.fileUpdated =  CommonTools::convertDate(str);
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
    qDebug() << "XMLHandler::fatalError:" << exception.message();
    return true;
}

bool XMLHandler::handleReply(const QString &qName, const QXmlAttributes &attribs, int queryType)
{
    QString itemType;

    switch(queryType)
    {
    case FOLDER_TYPE: itemType = FOLDER_TYPE_STR;
        break;

    case FILE_TYPE:itemType = FILE_TYPE_STR;
        break;
    }

    if(qName == CONTENT && itemType == FILE_TYPE_STR)
    {
        itemData.fileType = FYLE_TYPE_ATTRIBUTE_TAG;
        itemData.downloadLink = FYLE_TYPE_SRC_ATTRIBUTE_TAG;
    }

    if(qName == ENTRY_TAG) tags[EEntry] = true;
    if(qName == AUTHOR_TAG) tags[EAuthor]= true;
    if(qName == FOLDER_TITLE_TAG) tags[ETitle]= true;
    if(qName == FILE_SIZE_TAG) tags[ESize] = true;
    if(qName == UPDATED_FILE_TAG) tags[EUpdated] = true;

    if(HIERARCHY_ATTRIBUTE_TAG == PARENT_TAG)
    {
        itemData.parent = HIERARCHY_VALUE_TAG;
    }

    if(HIERARCHY_ATTRIBUTE_TAG == UPLOAD_TAG) itemData.uploadLink = HIERARCHY_VALUE_TAG;

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
        itemData.self = HIERARCHY_VALUE_TAG;
        itemData.type = itemType;
        itemInfo->push_back(itemData, queryType);
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
    itemData.fileSize = "---";
    itemData.fileUpdated = "";
    itemData.parent = ROOT_TAG;
    //itemData.self = "";
}

bool XMLHandler::resDownloadingNow(void) const
{
    return isResDownloding;
}



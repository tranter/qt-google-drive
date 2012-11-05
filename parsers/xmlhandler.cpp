#include "xmlhandler.h"
#include "share/commontools.h"
#include "share/debug.h"

XMLHandler::XMLHandler(int type):
    itemInfo(new ItemInfo),
    queryType(type),
    resDownloadedCount(0),
    isResDownloding(false)
{
    for(int i = ETitle; i < ETagsCount; ++i)
    {
        tags[i] = false;
    }
}

XMLHandler::~XMLHandler()
{
}

bool XMLHandler::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &attribs)
{
    return handleReply(qName, attribs);
}

bool XMLHandler::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
    return true;
}

bool XMLHandler::characters(const QString &str)
{
    if(tags[EAuthor]) handleAuthorTag(str, tags[EEntry]);
    if(tags[ETitle]) handleTitleTag(str);
    if(tags[ESize]) handleSizeTag(str);
    if(tags[EUpdated]) handleUpdatedTag(str);

    return true;
}

void XMLHandler::handleAuthorTag(const QString &str, bool entryTag)
{
    int index;

    if(entryTag)
    {
        switch(queryType)
        {
        case FOLDER_TYPE:
        {
            index = itemInfo->getItems().count() - 1;
        }
            break;
        case FILE_TYPE:
        {
            index = itemInfo->getFileItems().count() - 1;
        }
            break;
        }

        QString author(str);

        if(itemInfo->getAccountOwner() == author) author = OWNER_ME;
        itemInfo->setDataOwner(author, index, queryType);
    }
    else
    {
        itemInfo->setAccountOwner(str);
    }

    tags[EAuthor] = false;
    tags[EEntry] = false;
}

void XMLHandler::handleTitleTag(const QString &str)
{
    itemData.name = str;
    tags[ETitle] = false;
}

void XMLHandler::handleSizeTag(const QString &str)
{
    itemInfo->setFileSize(CommonTools::getFormattedFileSize(str), itemInfo->getFileItems().count() - 1);
    tags[ESize] = false;
}

void XMLHandler::handleUpdatedTag(const QString &str)
{
    itemData.fileUpdated =  CommonTools::convertDate(str);
    tags[EUpdated] = false;
}

ItemInfo* XMLHandler::getItemInfo(void) const
{
    DEBUG;
    return itemInfo.data();
}

bool XMLHandler::fatalError(const QXmlParseException &exception)
{
    DEBUG << "fatalError:" << exception.message();
    return true;
}

bool XMLHandler::handleReply(const QString &qName, const QXmlAttributes &attribs)
{
    QString itemType;

    setItemType(itemType);
    setTag(qName, true);

    if(qName == CONTENT && itemType == FILE_TYPE_STR)
    {
        itemData.fileType = FYLE_TYPE_ATTRIBUTE_TAG(attribs.value);
        itemData.downloadLink = FYLE_TYPE_SRC_ATTRIBUTE_TAG(attribs.value);
    }

    if(HIERARCHY_ATTRIBUTE_TAG(attribs.value) == PARENT_TAG) itemData.parent = HIERARCHY_VALUE_TAG(attribs.value);
    if(HIERARCHY_ATTRIBUTE_TAG(attribs.value) == UPLOAD_TAG) itemData.uploadLink = HIERARCHY_VALUE_TAG(attribs.value);
    if(HIERARCHY_ATTRIBUTE_TAG(attribs.value) == ICON_TAG) saveResData(attribs);

    if(HIERARCHY_ATTRIBUTE_TAG(attribs.value) == SELF_TAG)
    {
        itemData.self = HIERARCHY_VALUE_TAG(attribs.value);
        itemData.type = itemType;
        itemInfo->push_back(itemData, queryType);

        setDefaults();
    }

    return true;
}

void XMLHandler::setItemType(QString &itemType)
{
    switch(queryType)
    {
    case FOLDER_TYPE: itemType = FOLDER_TYPE_STR;
        break;

    case FILE_TYPE:itemType = FILE_TYPE_STR;
        break;
    }
}

void XMLHandler::setTag(const QString &qName, bool state)
{
    if(qName == ENTRY_TAG) tags[EEntry] = state;
    if(qName == AUTHOR_TAG) tags[EAuthor]= state;
    if(qName == FOLDER_TITLE_TAG) tags[ETitle]= state;
    if(qName == FILE_SIZE_TAG) tags[ESize] = state;
    if(qName == UPDATED_FILE_TAG) tags[EUpdated] = state;
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

void XMLHandler::saveResData(const QXmlAttributes &attribs)
{
    if(!CommonTools::fileFromURLExists(HIERARCHY_VALUE_TAG(attribs.value)))
    {
        isResDownloding = true;

        resManagers.push_back(new ResManager);
        resManagers.last()->cash(HIERARCHY_VALUE_TAG(attribs.value));

        connect(resManagers.last(), SIGNAL(signalResDownloaded()), this, SLOT(slotResDownloaded()));
    }

    itemData.iconPath = CommonTools::getFileNameFromURL(HIERARCHY_VALUE_TAG(attribs.value));
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
}

bool XMLHandler::resDownloadingNow(void) const
{
    return isResDownloding;
}



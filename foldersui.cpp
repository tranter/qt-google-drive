#include "foldersui.h"

FoldersUI::FoldersUI(QObject *parent) :
    QObject(parent)
{
}

bool FoldersUI::getFolderContent(QString &folderID)
{
    QList<ItemInfo::Data> item = SDriveEngine::inst()->filesMngr->getParser()->getXMLHandler()->getItemInfo()->getFileItems();
    int index = SDriveEngine::inst()->filesUI->getCurrFileItemId(SDriveEngine::inst()->filesMngr.data());
    bool isFolder = false;

    QString str(item[index].self);
    QStringList strList = str.split("/");
    str = strList[strList.count() - 1];

    if(str.indexOf(FOLDER_TYPE_STR) != -1)
    {
        folderID = str;
        isFolder = true;
    }

    return isFolder;
}

int FoldersUI::getCurrFolderItemId(void) const
{
    QTreeWidgetItem* pointer = static_cast<QTreeWidgetItem*>(SUi::inst()->treeFoldersView->currentIndex().internalPointer());
    ItemInfo item = *SDriveEngine::inst()->foldersMngr->getParser()->getXMLHandler()->getItemInfo();
    int count = item.getItems().count();

    int currentModelIndex = 0;

    for(int i = 0; i < count; ++i)
    {
        if(item[i].pointer == pointer)
        {
            currentModelIndex = i;
            break;
        }
    }

    return currentModelIndex;
}

void FoldersUI::createAFolders(void)
{
    QString generalImage(":ico/allitems");

    SDriveEngine::inst()->aFoldersMngr->create(ALL_ITEMS_TITLE, generalImage);
    SDriveEngine::inst()->aFoldersMngr->create(GET_USER_DOCUMENTS_TITLE, generalImage);
    SDriveEngine::inst()->aFoldersMngr->create(GET_USER_PRESENTATIONS_TITLE, generalImage);
    SDriveEngine::inst()->aFoldersMngr->create(GET_USER_SPREADSHEETS_TITLE, generalImage);
    SDriveEngine::inst()->aFoldersMngr->create(OWNED_BY_ME_TITLE, generalImage);
    SDriveEngine::inst()->aFoldersMngr->create(GET_STARRED_TITLE, generalImage);
    SDriveEngine::inst()->aFoldersMngr->create(TRASH_TITLE, ":ico/trash");
}

void FoldersUI::showFolders(const QString& url)
{
    SDriveEngine::inst()->foldersMngr->get(url);
}

void FoldersUI::slotFoldersViewClicked(const QModelIndex& index)
{
    SDriveEngine::inst()->elStates[DriveEngine::EFoldersTreeViewFocused] = true;
    SDriveEngine::inst()->elStates[DriveEngine::EAFoldersViewFocused] = false;
    SDriveEngine::inst()->elStates[DriveEngine::ERightViewFocused] = false;
    SDriveEngine::inst()->elStates[DriveEngine::ETrashFocused] = false;

    SDriveEngine::inst()->aFoldersMngr->clear();
    SDriveEngine::inst()->filesUI->showFiles();
}

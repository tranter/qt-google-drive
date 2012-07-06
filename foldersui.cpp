#include "foldersui.h"

FoldersUI::FoldersUI(QObject *parent) :
    QObject(parent)
{
}

bool FoldersUI::folderInFilesView(QString& resourceID)
{
    QList<TreeItemInfo::Data> item = SDriveEngine::inst()->filesManager->getParser()->getXMLHandler()->getTreeItemInfo()->getFileItems();
    int index = SDriveEngine::inst()->filesUI->getCurrentFileItemIndex(SDriveEngine::inst()->filesManager.data());
    bool isFolder = false;

    QString str(item[index].self);
    QStringList strList = str.split("/");
    str = strList[strList.count() - 1];

    if(str.indexOf(FOLDER_TYPE_STR) != -1)
    {
        resourceID = str;
        isFolder = true;
    }

    return isFolder;
}

int FoldersUI::getCurrentFolderItemIndex(void) const
{
    QTreeWidgetItem* pointer = static_cast<QTreeWidgetItem*>(SUi::inst()->foldersView->currentIndex().internalPointer());
    TreeItemInfo item = *SDriveEngine::inst()->foldersManager->getParser()->getXMLHandler()->getTreeItemInfo();
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

void FoldersUI::showAdditionalFolders(void)
{
    QString generalImage(":ico/allitems");

    SDriveEngine::inst()->additionalFilesManager->create(ALL_ITEMS_TITLE, generalImage);
    SDriveEngine::inst()->additionalFilesManager->create(GET_USER_DOCUMENTS_TITLE, generalImage);
    SDriveEngine::inst()->additionalFilesManager->create(GET_USER_PRESENTATIONS_TITLE, generalImage);
    SDriveEngine::inst()->additionalFilesManager->create(GET_USER_SPREADSHEETS_TITLE, generalImage);
    SDriveEngine::inst()->additionalFilesManager->create(OWNED_BY_ME_TITLE, generalImage);
    SDriveEngine::inst()->additionalFilesManager->create(GET_STARRED_TITLE, generalImage);
    SDriveEngine::inst()->additionalFilesManager->create(TRASH_TITLE, ":ico/trash");
}

void FoldersUI::showFolders(void)
{
    SDriveEngine::inst()->foldersManager.reset(new FoldersManager);
    SDriveEngine::inst()->foldersManager->get(GET_FOLDERS);
}

void FoldersUI::slotFoldersViewClicked(const QModelIndex& index)
{
    SDriveEngine::inst()->currentFolderIndex = index.row();

    SDriveEngine::inst()->elementsStates[DriveEngine::EFolderViewFocused] = true;
    SDriveEngine::inst()->elementsStates[DriveEngine::EAdditionalViewFocused] = false;
    SDriveEngine::inst()->elementsStates[DriveEngine::EFilesViewFocused] = false;
    SDriveEngine::inst()->elementsStates[DriveEngine::ETrashFocused] = false;

    SDriveEngine::inst()->additionalFilesManager->clear();
    SDriveEngine::inst()->filesUI->showFiles();
}

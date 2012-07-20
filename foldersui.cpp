#include "foldersui.h"
#include <QDebug>

FoldersUI::FoldersUI(QObject *parent) :
    QObject(parent)
{
}

bool FoldersUI::folderInFilesView(QString& resourceID)
{
    QList<ItemInfo::Data> item = SDriveEngine::inst()->filesMngr->getParser()->getXMLHandler()->getItemInfo()->getFileItems();
    int index = SDriveEngine::inst()->filesUI->getCurrentFileItemIndex(SDriveEngine::inst()->filesMngr.data());
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

void FoldersUI::showAdditionalFolders(void)
{
    QString generalImage(":ico/allitems");

    SDriveEngine::inst()->addlFilesMngr->create(ALL_ITEMS_TITLE, generalImage);
    SDriveEngine::inst()->addlFilesMngr->create(GET_USER_DOCUMENTS_TITLE, generalImage);
    SDriveEngine::inst()->addlFilesMngr->create(GET_USER_PRESENTATIONS_TITLE, generalImage);
    SDriveEngine::inst()->addlFilesMngr->create(GET_USER_SPREADSHEETS_TITLE, generalImage);
    SDriveEngine::inst()->addlFilesMngr->create(OWNED_BY_ME_TITLE, generalImage);
    SDriveEngine::inst()->addlFilesMngr->create(GET_STARRED_TITLE, generalImage);
    SDriveEngine::inst()->addlFilesMngr->create(TRASH_TITLE, ":ico/trash");
}

void FoldersUI::showFolders(void)
{
    SDriveEngine::inst()->foldersMngr->get(GET_FOLDERS);
}

void FoldersUI::slotFoldersViewClicked(const QModelIndex& index)
{
    qDebug() << "FoldersUI::slotFoldersViewClicked";
    //SDriveEngine::inst()->getFoldersUI()->currentFolderIndex = index.row();

    SDriveEngine::inst()->elStates[DriveEngine::EFolderViewFocused] = true;
    SDriveEngine::inst()->elStates[DriveEngine::EAddViewFocused] = false;
    SDriveEngine::inst()->elStates[DriveEngine::EFilesViewFocused] = false;
    SDriveEngine::inst()->elStates[DriveEngine::ETrashFocused] = false;

    SDriveEngine::inst()->addlFilesMngr->clear();
    SDriveEngine::inst()->filesUI->showFiles();
}

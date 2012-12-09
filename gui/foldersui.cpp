#include "foldersui.h"
#include "share/debug.h"

FoldersUI::FoldersUI(QObject *parent) :
    QObject(parent)
{
}

QString FoldersUI::getFolderID(void)
{  
    QString  str(SDriveEngine::inst()->getFilesMngr()->getCurrentFileInfo().self);
    QStringList strList = str.split("/");

    return strList[strList.count() - 1];
}

bool FoldersUI::isFolder(void)
{
    bool is = false;
    QString  str(SDriveEngine::inst()->getFilesMngr()->getCurrentFileInfo().self);
    QStringList strList = str.split("/");

    str = strList[strList.count() - 1];

    if(str.indexOf(FOLDER_TYPE_STR) != -1) is = true;

    DEBUG << "is:" << is;

    return is;
}

//ItemInfo::Data FoldersUI::item(void)
//{
//    QList<ItemInfo::Data> items = SDriveEngine::inst()->getFilesMngr()->getParser()->getXMLHandler()->getItemInfo()->getFileItems();
//    int index = SDriveEngine::inst()->filesUI->getCurrFileItemId(SDriveEngine::inst()->getFilesMngr());

//    return items[index];
//}

//int FoldersUI::getCurrFolderItemId(void) const
//{
//    QTreeWidgetItem* pointer = static_cast<QTreeWidgetItem*>(SUi::inst()->treeFoldersView->currentIndex().internalPointer());
//    ItemInfo item = *SDriveEngine::inst()->foldersMngr->getParser()->getXMLHandler()->getItemInfo();
//    int count = item.getItems().count();

//    int currentModelIndex = 0;

//    for(int i = 0; i < count; ++i)
//    {
//        if(item[i].pointer == pointer)
//        {
//            currentModelIndex = i;
//            break;
//        }
//    }

//    DEBUG << "currentModelIndex" << currentModelIndex;

//    return currentModelIndex;
//}

//void FoldersUI::createAFolders(void)
//{
//    QString generalImage(":ico/allitems");

//    SDriveEngine::inst()->aFoldersMngr->create(ALL_ITEMS_TITLE, generalImage);
//    SDriveEngine::inst()->aFoldersMngr->create(GET_USER_DOCUMENTS_TITLE, generalImage);
//    SDriveEngine::inst()->aFoldersMngr->create(GET_USER_PRESENTATIONS_TITLE, generalImage);
//    SDriveEngine::inst()->aFoldersMngr->create(GET_USER_SPREADSHEETS_TITLE, generalImage);
//    SDriveEngine::inst()->aFoldersMngr->create(OWNED_BY_ME_TITLE, generalImage);
//    SDriveEngine::inst()->aFoldersMngr->create(GET_STARRED_TITLE, generalImage);
//    SDriveEngine::inst()->aFoldersMngr->create(TRASH_TITLE, ":ico/trash");
//}

//void FoldersUI::showFolders(const QString& url)
//{
//    SDriveEngine::inst()->foldersMngr->get(url);
//}

//void FoldersUI::slotFoldersViewClicked(const QModelIndex& index)
//{
//    Q_UNUSED(index);

//    SDriveEngine::inst()->elStates[EFoldersTreeViewFocused] = true;
//    SDriveEngine::inst()->elStates[EAFoldersViewFocused] = false;
//    SDriveEngine::inst()->elStates[ERightViewFocused] = false;
//    SDriveEngine::inst()->elStates[ETrashFocused] = false;

//    SDriveEngine::inst()->aFoldersMngr->clear();
//    SDriveEngine::inst()->filesUI->showFiles();
//}

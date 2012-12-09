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

    if(str.indexOf(FOLDER_TYPE_STR) != -1)
    {
        is = true;
    }

    return is;
}

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

#include "filesui.h"
#include <QDebug>

FilesUI::FilesUI(QObject *parent) :
    QObject(parent)
{
}

int FilesUI::getCurrFileItemId(FilesManager* manager) const
{
    QList<ItemInfo::Data> item = manager->getParser()->getXMLHandler()->getItemInfo()->getFileItems();
    int count = item.count();
    QString fileName(SUi::inst()->filesView->currentIndex().data().toString());

    int currentFileIndex = 0;

    for(int i = 1; i < count; ++i)
    {
        if(fileName == item[i].name)
        {
            currentFileIndex = i;
            break;
        }
    }

    return currentFileIndex;
}

void FilesUI::showFiles(void)
{
    ItemInfo item = *SDriveEngine::inst()->foldersMngr->getParser()->getXMLHandler()->getItemInfo();
    int itemsIndex = SDriveEngine::inst()->foldersUI->getCurrFolderItemId();

    if (item.getItems().size() > 0)
    {
        if(item[itemsIndex].type == FOLDER_TYPE_STR)
        {
            QString query(item[itemsIndex].self);
            query += QString(CONTENTS + MAX_RESULTS);

            SDriveEngine::inst()->filesMngr->get(query);
        }
    }
}

void FilesUI::showFilesFromFolder(void)
{
    QString str;

    if(SDriveEngine::inst()->foldersUI->folderInFilesView(str))
    {
        QString query(GET_FILES_IN_FOLDER);
        query += str;
        query += QString(CONTENTS + MAX_RESULTS);

        qDebug() << "FilesUI::showFilesFromFolderInFilesView query:" << query;

        SDriveEngine::inst()->filesMngr->get(query);
    }
}

void FilesUI::slotAShowFiles(const QModelIndex& index)
{
    if(index.model()->data(index).toString() == TRASH_TITLE) SDriveEngine::inst()->elStates[DriveEngine::ETrashFocused] = true;
    else SDriveEngine::inst()->elStates[DriveEngine::ETrashFocused] = false;

    QString query;
    SDriveEngine::inst()->getFoldersUI()->currAFolderId = index;

    SDriveEngine::inst()->elStates[DriveEngine::EFolderViewFocused] = false;
    SDriveEngine::inst()->elStates[DriveEngine::EAViewFocused] = true;
    SDriveEngine::inst()->elStates[DriveEngine::EFilesViewFocused] = false;

    SDriveEngine::inst()->filesMngr->clear();

    if(index.model()->data(index).toString() == ALL_ITEMS_TITLE) query = GET_ALL_ITEMS + MAX_RESULTS;
    if(index.model()->data(index).toString() == GET_USER_DOCUMENTS_TITLE) query = GET_USER_DOCUMENTS + MAX_RESULTS;
    if(index.model()->data(index).toString() == GET_USER_PRESENTATIONS_TITLE) query = GET_USER_PRESENTATIONS + MAX_RESULTS;
    if(index.model()->data(index).toString() == GET_USER_SPREADSHEETS_TITLE) query = GET_USER_SPREADSHEETS + MAX_RESULTS;
    if(index.model()->data(index).toString() == OWNED_BY_ME_TITLE) query = GET_OWNED_BY_ME + MAX_RESULTS;
    if(index.model()->data(index).toString() == GET_STARRED_TITLE)  query = GET_STARRED;
    if(index.model()->data(index).toString() == TRASH_TITLE) query = GET_TRASH;

    SDriveEngine::inst()->aFoldersMngr->get(query);
}

void FilesUI::slotFilesSortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
{
    qDebug() << "index:" << QString::number(logicalIndex) << " order:" << order;
}

void FilesUI::slotFilesViewClicked(const QModelIndex&)
{
    SDriveEngine::inst()->elStates[DriveEngine::EFolderViewFocused] = false;
    SDriveEngine::inst()->elStates[DriveEngine::EFilesViewFocused] = true;

    if(!SDriveEngine::inst()->elStates[DriveEngine::EAViewFocused]) showFilesFromFolder();
}

void FilesUI::slotUpdateFileList()
{
    showFiles();
}


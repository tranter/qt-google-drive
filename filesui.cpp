#include "filesui.h"
#include <QDebug>

FilesUI::FilesUI(QObject *parent) :
    QObject(parent)
{
}

int FilesUI::getCurrentFileItemIndex(FilesManager* manager) const
{
    QList<TreeItemInfo::Data> item = manager->getParser()->getXMLHandler()->getTreeItemInfo()->getFileItems();
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
    TreeItemInfo item = *SDriveEngine::inst()->foldersManager->getParser()->getXMLHandler()->getTreeItemInfo();
    int treeItemsIndex = SDriveEngine::inst()->foldersUI->getCurrentFolderItemIndex();

    if (item.getItems().size() > 0) {
        if(item[treeItemsIndex].type == FOLDER_TYPE_STR)
        {
            QString query(item[treeItemsIndex].self);
            query += QString(CONTENTS + MAX_RESULTS);

            SDriveEngine::inst()->filesManager->get(query);
        }
    }
}

void FilesUI::showFilesFromFolderInFilesView(void)
{
    QString str;

    if(SDriveEngine::inst()->foldersUI->folderInFilesView(str))
    {
        QString query(GET_FILES_IN_FOLDER);
        query += str;
        query += QString(CONTENTS + MAX_RESULTS);

        SDriveEngine::inst()->filesManager->get(query);
    }
}

void FilesUI::slotAdditionalShowFiles(const QModelIndex& index)
{
    if(index.model()->data(index).toString() == TRASH_TITLE) SDriveEngine::inst()->elementsStates[DriveEngine::ETrashFocused] = true;
    else SDriveEngine::inst()->elementsStates[DriveEngine::ETrashFocused] = false;

    QString query;
    SDriveEngine::inst()->getFoldersUI()->currentAdditionalFolderIndex = index;

    SDriveEngine::inst()->elementsStates[DriveEngine::EFolderViewFocused] = false;
    SDriveEngine::inst()->elementsStates[DriveEngine::EAdditionalViewFocused] = true;
    SDriveEngine::inst()->elementsStates[DriveEngine::EFilesViewFocused] = false;

    SDriveEngine::inst()->filesManager->clear();

    if(index.model()->data(index).toString() == ALL_ITEMS_TITLE) query = GET_ALL_ITEMS + MAX_RESULTS;
    if(index.model()->data(index).toString() == GET_USER_DOCUMENTS_TITLE) query = GET_USER_DOCUMENTS + MAX_RESULTS;
    if(index.model()->data(index).toString() == GET_USER_PRESENTATIONS_TITLE) query = GET_USER_PRESENTATIONS + MAX_RESULTS;
    if(index.model()->data(index).toString() == GET_USER_SPREADSHEETS_TITLE) query = GET_USER_SPREADSHEETS + MAX_RESULTS;
    if(index.model()->data(index).toString() == OWNED_BY_ME_TITLE) query = GET_OWNED_BY_ME + MAX_RESULTS;
    if(index.model()->data(index).toString() == GET_STARRED_TITLE)  query = GET_STARRED;
    if(index.model()->data(index).toString() == TRASH_TITLE) query = GET_TRASH;

    SDriveEngine::inst()->additionalFilesManager->get(query);
}

void FilesUI::slotFilesSortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
{
    qDebug() << "index:" << QString::number(logicalIndex) << " order:" << order;
}

void FilesUI::slotFilesViewClicked(const QModelIndex&)
{
    SDriveEngine::inst()->elementsStates[DriveEngine::EFolderViewFocused] = false;
    SDriveEngine::inst()->elementsStates[DriveEngine::EFilesViewFocused] = true;

    if(!SDriveEngine::inst()->elementsStates[DriveEngine::EAdditionalViewFocused]) showFilesFromFolderInFilesView();
}

void FilesUI::slotUpdateFileList()
{
    showFiles();
}


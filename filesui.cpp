#include "filesui.h"
#include "AppRegData.h"
#include <QSettings>
#include <QDebug>

FilesUI::FilesUI(QObject *parent) :
    QObject(parent)
{
}

int FilesUI::getCurrFileItemId(FilesManager* manager) const
{
    QList<ItemInfo::Data> item = manager->getParser()->getXMLHandler()->getItemInfo()->getFileItems();
    int count = item.count();
    QString fileName(SDriveEngine::inst()->getCurrFilesMngr()->getPanel()->currentIndex().data().toString());

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
            query += (CONTENTS + MAX_RESULTS);

            SDriveEngine::inst()->getCurrFilesMngr()->get(query);
        }
    }
}

void FilesUI::showFilesFromFolder(void)
{
    QString str;

    if(SDriveEngine::inst()->foldersUI->getFolderContent(str))
    {
        QString query(GET_FILES_IN_FOLDER);
        query += str;
        query += (CONTENTS + MAX_RESULTS);

        SDriveEngine::inst()->getCurrFilesMngr()->get(query);
    }
}

void FilesUI::slotAShowFiles(const QModelIndex& index)
{
    if(index.model()->data(index).toString() == TRASH_TITLE) SDriveEngine::inst()->elStates[DriveEngine::ETrashFocused] = true;
    else SDriveEngine::inst()->elStates[DriveEngine::ETrashFocused] = false;

    QString query;
    SDriveEngine::inst()->getFoldersUI()->currAFolderId = index;

    SDriveEngine::inst()->elStates[DriveEngine::EFoldersTreeViewFocused] = false;
    SDriveEngine::inst()->elStates[DriveEngine::EAFoldersViewFocused] = true;
    SDriveEngine::inst()->elStates[DriveEngine::ERightViewFocused] = false;

    SDriveEngine::inst()->getCurrFilesMngr()->clear();

    if(index.model()->data(index).toString() == ALL_ITEMS_TITLE) query = GET_ALL_ITEMS + MAX_RESULTS;
    if(index.model()->data(index).toString() == GET_USER_DOCUMENTS_TITLE) query = GET_USER_DOCUMENTS + MAX_RESULTS;
    if(index.model()->data(index).toString() == GET_USER_PRESENTATIONS_TITLE) query = GET_USER_PRESENTATIONS + MAX_RESULTS;
    if(index.model()->data(index).toString() == GET_USER_SPREADSHEETS_TITLE) query = GET_USER_SPREADSHEETS + MAX_RESULTS;
    if(index.model()->data(index).toString() == OWNED_BY_ME_TITLE) query = GET_OWNED_BY_ME + MAX_RESULTS;
    if(index.model()->data(index).toString() == GET_STARRED_TITLE)  query = GET_STARRED;
    if(index.model()->data(index).toString() == TRASH_TITLE) query = GET_TRASH;

    SDriveEngine::inst()->aFoldersMngr->get(query);
}

void FilesUI::slotLeftSortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
{
    qDebug() << "FilesUI::slotLeftSortIndicatorChanged index:" << QString::number(logicalIndex) << " order:" << order;
}

void FilesUI::slotRightSortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
{
    qDebug() << "FilesUI::slotRightSortIndicatorChanged index:" << QString::number(logicalIndex) << " order:" << order;
}

void FilesUI::slotLeftViewClicked(const QModelIndex& Id)
{
    QSettings settings(COMPANY_NAME, APP_NAME);
    settings.setValue(CURRENT_PANEL, LEFT_PANEL);

    showFilesOnPanel(Id);
}

void FilesUI::slotRightViewClicked(const QModelIndex& Id)
{
    QSettings settings(COMPANY_NAME, APP_NAME);
    settings.setValue(CURRENT_PANEL, RIGHT_PANEL);

    showFilesOnPanel(Id);
}

void FilesUI::showFilesOnPanel(const QModelIndex& Id)
{
    qDebug() << "showFilesOnPanel:" << Id.data().toString();

    SDriveEngine::inst()->elStates[DriveEngine::EFoldersTreeViewFocused] = false;
    SDriveEngine::inst()->elStates[DriveEngine::ERightViewFocused] = true;

    if(Id.data().toString() == PARENT_FOLDER_SIGN)
    {
        qDebug() << "PARENT:" << SDriveEngine::inst()->getCurrFilesMngr()->getRequest().url().toString();
        SDriveEngine::inst()->getCurrFilesMngr()->get(SDriveEngine::inst()->getCurrFilesMngr()->prevLink());
    }
    else
    {
        if(!SDriveEngine::inst()->elStates[DriveEngine::EAFoldersViewFocused]) showFilesFromFolder();
    }
}

void FilesUI::slotUpdateFileList()
{
    showFiles();
}


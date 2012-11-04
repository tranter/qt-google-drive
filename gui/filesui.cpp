#include "filesui.h"
#include "share/appregdata.h"
#include <QSettings>
#include "share/debug.h"

FilesUI::FilesUI(QObject *parent) :
    QObject(parent)
{
}

int FilesUI::getCurrFileItemId(FilesManager* manager) const
{
    QList<ItemInfo::Data> item = manager->getParser()->getXMLHandler()->getItemInfo()->getFileItems();
    int count = item.count();
    QString fileName(SDriveEngine::inst()->getCurrFilesMngr()->getPanel()->currentIndex().data().toString());

    DEBUG << "!!!!!!!!!---->" << fileName;

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
    DEBUG;

    if(SDriveEngine::inst()->foldersUI->isFolder())
    {
        QString query(GET_FILES_IN_FOLDER);
        query += SDriveEngine::inst()->foldersUI->getFolderID();
        query += (CONTENTS + MAX_RESULTS);

        SDriveEngine::inst()->getCurrFilesMngr()->get(query);
    }
}

void FilesUI::slotAShowFiles(const QModelIndex& index)
{
    if(index.model()->data(index).toString() == TRASH_TITLE)
    {
        SDriveEngine::inst()->elStates[ETrashFocused] = true;
    }
    else
    {
        SDriveEngine::inst()->elStates[ETrashFocused] = false;
    }

    QString query;
    SDriveEngine::inst()->getFoldersUI()->currAFolderId = index;

    SDriveEngine::inst()->elStates[EFoldersTreeViewFocused] = false;
    SDriveEngine::inst()->elStates[EAFoldersViewFocused] = true;
    SDriveEngine::inst()->elStates[ERightViewFocused] = false;

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
}

void FilesUI::slotRightSortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
{
}

void FilesUI::slotLeftViewClicked(const QModelIndex &Id)
{
}

void FilesUI::slotRightViewClicked(const QModelIndex &Id)
{
}

void FilesUI::slotLeftPanelItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    DEBUG << "item" << item->data(0, Qt::DisplayRole).toString();

    QSettings(COMPANY_NAME, APP_NAME).setValue(CURRENT_PANEL, LEFT_PANEL);
    showFilesOnPanel(item->data(0, Qt::DisplayRole).toString(), ELeft);
}

void FilesUI::slotRightPanelItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    DEBUG << "item %s" << item->data(0, Qt::DisplayRole).toString().toAscii();

    QSettings(COMPANY_NAME, APP_NAME).setValue(CURRENT_PANEL, RIGHT_PANEL);
    showFilesOnPanel(item->data(0, Qt::DisplayRole).toString(), ERight);
}

void FilesUI::showFilesOnPanel(const QString &Id, EPanels panel)
{
    DEBUG << "panel:" << panel << "name:" << SDriveEngine::inst()->getFoldersUI()->item().name << "item().self:" << SDriveEngine::inst()->getFoldersUI()->item().self;

    SDriveEngine::inst()->elStates[EFoldersTreeViewFocused] = false;
    SDriveEngine::inst()->elStates[ERightViewFocused] = true;

    if(Id == PARENT_FOLDER_SIGN)
    {
        setPanelDisplayingPath(Id, EBackward, panel);
        SDriveEngine::inst()->getCurrFilesMngr()->get(SDriveEngine::inst()->getCurrFilesMngr()->back());
    }
    else
    {
        if(SDriveEngine::inst()->foldersUI->isFolder())
        {
            setPanelDisplayingPath(Id, EForward, panel);
        }

        if(!SDriveEngine::inst()->elStates[EAFoldersViewFocused])
        {
            showFilesFromFolder();
        }
    }
}

void FilesUI::slotUpdateFileList()
{
    DEBUG;
    //showFiles();
    SDriveEngine::inst()->getCurrFilesMngr()->get(SDriveEngine::inst()->getCurrFilesMngr()->getCurrLink());
}

void FilesUI::setPanelDisplayingPath(const QString &name, EPath path, EPanels panel)
{
    QLabel* label = getPanelLabel(panel);
    QString pathDividerSign("\\");
    int labelTextlength = label->text().length();
    int discStrLength = getDisc(panel).length();

    switch(path)
    {
    case EForward:
    {
        QString divider((labelTextlength == discStrLength) ? "" : pathDividerSign);
        label->setText(label->text() += divider + name);
    }
        break;
    case EBackward:
    {
        int pos = label->text().lastIndexOf(pathDividerSign);
        int removeCount = labelTextlength - (pos + 1) + 1;

        if(pos == (discStrLength - 1)) ++pos;

        label->setText(label->text().remove(pos,  removeCount));
    }
        break;
    }
}

QString FilesUI::getDisc(EPanels panel) const
{
    QString disc;

    disc = "a:\\";

    return disc;
}

void FilesUI::setDisplayingDisc(EPanels panel)
{
    getPanelLabel(panel)->setText(getDisc(panel));
}

QLabel* FilesUI::getPanelLabel(EPanels panel) const
{
    QLabel* label;

    switch(panel)
    {
    case ELeft: label = SUi::inst()->pathLabelLeft;
        break;
    case ERight: label = SUi::inst()->pathLabelRight;
        break;
    case EPanelsCount:
        break;
    }

    return label;
}



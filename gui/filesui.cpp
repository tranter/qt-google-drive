#include <QSettings>
#include <QDir>
#include "filesui.h"
#include "share/registration.h"
#include "share/debug.h"

FilesUI::FilesUI(QObject *parent) :
    QObject(parent)
{
}

//void FilesUI::slotAShowFiles(const QModelIndex& index)
//{
//    if(index.model()->data(index).toString() == TRASH_TITLE)
//    {
//        SDriveEngine::inst()->elStates[ETrashFocused] = true;
//    }
//    else
//    {
//        SDriveEngine::inst()->elStates[ETrashFocused] = false;
//    }

//    QString query;
//    SDriveEngine::inst()->getFoldersUI()->currAFolderId = index;

//    SDriveEngine::inst()->elStates[EFoldersTreeViewFocused] = false;
//    SDriveEngine::inst()->elStates[EAFoldersViewFocused] = true;
//    SDriveEngine::inst()->elStates[ERightViewFocused] = false;

//    SDriveEngine::inst()->getFilesMngr()->clear();

//    if(index.model()->data(index).toString() == ALL_ITEMS_TITLE) query = GET_ALL_ITEMS + MAX_RESULTS;
//    if(index.model()->data(index).toString() == GET_USER_DOCUMENTS_TITLE) query = GET_USER_DOCUMENTS + MAX_RESULTS;
//    if(index.model()->data(index).toString() == GET_USER_PRESENTATIONS_TITLE) query = GET_USER_PRESENTATIONS + MAX_RESULTS;
//    if(index.model()->data(index).toString() == GET_USER_SPREADSHEETS_TITLE) query = GET_USER_SPREADSHEETS + MAX_RESULTS;
//    if(index.model()->data(index).toString() == OWNED_BY_ME_TITLE) query = GET_OWNED_BY_ME + MAX_RESULTS;
//    if(index.model()->data(index).toString() == GET_STARRED_TITLE)  query = GET_STARRED;
//    if(index.model()->data(index).toString() == TRASH_TITLE) query = GET_TRASH;

//    SDriveEngine::inst()->aFoldersMngr->get(query);
//}

void FilesUI::slotLeftSortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
{
}

void FilesUI::slotRightSortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
{
}

void FilesUI::slotLeftViewClicked(const QModelIndex &Id)
{
    QSettings(COMPANY_NAME, APP_NAME).setValue(CURRENT_PANEL, LEFT_PANEL);
}

void FilesUI::slotRightViewClicked(const QModelIndex &Id)
{
    QSettings(COMPANY_NAME, APP_NAME).setValue(CURRENT_PANEL, RIGHT_PANEL);
}

void FilesUI::slotLeftPanelItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    DEBUG << "item" << item->data(0, Qt::DisplayRole).toString();

    QSettings(COMPANY_NAME, APP_NAME).setValue(CURRENT_PANEL, LEFT_PANEL);
    showFilesOnPanel(item->data(0, Qt::DisplayRole).toString(), ELeft);
}

void FilesUI::slotRightPanelItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    DEBUG << "item" << item->data(0, Qt::DisplayRole).toString().toAscii();

    QSettings(COMPANY_NAME, APP_NAME).setValue(CURRENT_PANEL, RIGHT_PANEL);
    showFilesOnPanel(item->data(0, Qt::DisplayRole).toString(), ERight);
}

void FilesUI::showFilesOnPanel(const QString &name, EPanels panel)
{
    if(name == PARENT_FOLDER_SIGN)
    {
        performShowFiles(SDriveEngine::inst()->getFilesMngr()->back(), name, EBackward, panel);
    }
    else
    {
        if(SDriveEngine::inst()->foldersUI->isFolder())
        {
            QString query(GET_FILES_IN_FOLDER);

            query += SDriveEngine::inst()->foldersUI->getFolderID();
            query += (CONTENTS + MAX_RESULTS);

            performShowFiles(query, name, EForward, panel);
        }
    }
}

void FilesUI::setCurrentPanelState(EPanels panel, const QString &URL)
{
    QSettings settings(COMPANY_NAME, APP_NAME);

    settings.beginGroup(QString("Panel-") + QString::number(static_cast <int> (panel)));

    settings.setValue(QString("Current Folder URL"), URL);
    settings.setValue(QString("Current Folder Path"), getPanelLabel(panel)->text());
    settings.setValue(QString("Path Links"), SDriveEngine::inst()->getFilesMngr()->getPathesURLs());

    settings.endGroup();
}

void FilesUI::performShowFiles(const QString &query, const QString &name, EPath path, EPanels panel)
{
    setPanelDisplayingPath(name, path, panel);
    SDriveEngine::inst()->getFilesMngr()->get(query);
    setCurrentPanelState(panel, query);
}

void FilesUI::slotUpdateFileList()
{
    SDriveEngine::inst()->getFilesMngr()->get(SDriveEngine::inst()->getFilesMngr()->getUpperLevelFolderURL());
}

void FilesUI::setPanelDisplayingPath(const QString &name, EPath path, EPanels panel)
{
    QLabel* label = getPanelLabel(panel);
    QString pathDividerSign(QDir::toNativeSeparators("/"));
    int labelTextlength = label->text().length();
    int discStrLength = getDisc(panel).length();

    switch(path)
    {
    case EForward:
    {
        QString divider((labelTextlength == discStrLength) ? "" : pathDividerSign);
        label->setText(label->text() += (divider + name));
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
    disc = QString("a:") + QDir::toNativeSeparators("/");
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



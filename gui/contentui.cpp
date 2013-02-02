#include "settings/settingsmanager.h"
#include "contentui.h"
#include "share/debug.h"
#include <QDir>
#include <QMouseEvent>

ContentUI::ContentUI(QObject *parent) :
    QObject(parent)
{
}

void ContentUI::slotItemLeftPressed(QTreeWidgetItem *item, int column)
{   
    Q_UNUSED(column);

    SettingsManager().setCurrentPanel(LEFT_PANEL_VALUE);

    if(QApplication::mouseButtons() == Qt::RightButton) markItem(item);
    if(QApplication::keyboardModifiers() == Qt::ControlModifier && QApplication::mouseButtons() == Qt::LeftButton) markItem(item, true);
}

void ContentUI::slotItemRightPressed(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);

    SettingsManager().setCurrentPanel(RIGHT_PANEL_VALUE);

    if(QApplication::mouseButtons() == Qt::RightButton) markItem(item);
    if(QApplication::keyboardModifiers() == Qt::ControlModifier && QApplication::mouseButtons() == Qt::LeftButton) markItem(item, true);
}

void ContentUI::slotLeftPanelItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);

    SettingsManager().setCurrentPanel(LEFT_PANEL_VALUE);
    showFilesOnPanel(item, ELeft);
}

void ContentUI::slotRightPanelItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);

    SettingsManager().setCurrentPanel(RIGHT_PANEL_VALUE);
    showFilesOnPanel(item, ERight);
}

void ContentUI::slotLeftCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    SettingsManager().setCurrentPanel(LEFT_PANEL_VALUE);

    if(previous)
    {
        if(QApplication::mouseButtons() == Qt::LeftButton && QApplication::keyboardModifiers() == Qt::ShiftModifier) markItems(current, previous);
    }
}

void ContentUI::slotRightCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    SettingsManager().setCurrentPanel(RIGHT_PANEL_VALUE);

    if(previous)
    {
        if(QApplication::mouseButtons() == Qt::LeftButton && QApplication::keyboardModifiers() == Qt::ShiftModifier) markItems(current, previous);
    }
}

int ContentUI::itemIndex(QTreeWidgetItem *item)
{
    int index = -1;

    for(int i = 0; i < item->treeWidget()->topLevelItemCount(); ++i)
    {
        if(item == item->treeWidget()->topLevelItem(i))
        {
            index = i;
            break;
        }
    }

    return index;
}

void ContentUI::markItem(QTreeWidgetItem *item, bool noSwitch)
{
    if(hasItemParentSign(item)) return;

    QBrush brush;

    if(noSwitch) brush.setColor(Qt::red);
    else brush.setColor(Qt::black);

    if(item->foreground(0).color() == Qt::black) brush.setColor(Qt::red);

    for(int i = 0; i < item->treeWidget()->columnCount(); ++i)
    {
        item->setForeground(i, brush);
    }
}

void ContentUI::markItems(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    int currentIndex = itemIndex(current);
    int previousIndex = itemIndex(previous);
    int beginIndex, endIndex;

    if(currentIndex < previousIndex)
    {
        beginIndex = currentIndex;
        endIndex = previousIndex;
    }
    else
    {
        beginIndex = previousIndex;
        endIndex = currentIndex;
    }

    for(int i = beginIndex; i <= endIndex; ++i)
    {
        markItem(current->treeWidget()->topLevelItem(i), true);
    }
}

QList<int> ContentUI::getMarkedItemIds(QTreeWidget *treeWidget) const
{
    QList<int> markedItemIds;
    int itemsCount = treeWidget->topLevelItemCount();
    int shiftItemsValue = 0;

    if(itemsCount > 0)
    {
        if(hasItemParentSign(treeWidget->topLevelItem(0))) shiftItemsValue = 1;
    }

    for(int i = 0; i < itemsCount; ++i)
    {
        if(treeWidget->topLevelItem(i)->foreground(0).color() == Qt::red) markedItemIds << (i - shiftItemsValue);
    }

    DEBUG << markedItemIds;

    return markedItemIds;
}

bool ContentUI::hasItemParentSign(QTreeWidgetItem *item) const
{
    return item->data(0, Qt::DisplayRole).toString() == PARENT_FOLDER_SIGN;
}

void ContentUI::showFilesOnPanel(QTreeWidgetItem *item, EPanels panel)
{
    const QString itemName(item->data(0, Qt::DisplayRole).toString());

    if(hasItemParentSign(item))
    {
        performShowFiles(SDriveEngine::inst()->getContentMngr()->back(), itemName, EBackward, panel);
    }
    else
    {
        if(isFolder())
        {
            QString query(GET_FILES_IN_FOLDER);

            query += CommonTools::getIDFromURL(SDriveEngine::inst()->getContentMngr()->getCurrentItem().self);
            query += (CONTENTS + MAX_RESULTS);

            performShowFiles(query, itemName, EForward, panel);
        }
    }
}

bool ContentUI::isFolder(void)
{
    return (SDriveEngine::inst()->getContentMngr()->getCurrentItem().type == FOLDER_TYPE_STR);
}

void ContentUI::setCurrentPanelState(EPanels panel, const QString &url)
{
    SettingsManager settingsManager;

    int panelNum = static_cast <int> (panel);

    settingsManager.setCurrentFolderURL(panelNum, url);

    QString fullPath(getPanelLabel(panel)->text());
    int beginPos = fullPath.indexOf(QDir::toNativeSeparators("/")) + 1;
    int length = fullPath.length() - beginPos;

    settingsManager.setCurrentFolderPath(panelNum, fullPath.mid(beginPos, length));
    settingsManager.setPathesURLs(panelNum, SDriveEngine::inst()->getContentMngr()->getPathesURLs());
}

void ContentUI::performShowFiles(const QString &query, const QString &name, EPath path, EPanels panel)
{
    setPanelDisplayingPath(name, path, panel);
    SDriveEngine::inst()->getContentMngr()->get(query);
    setCurrentPanelState(panel, query);

    children.fetch();
}

void ContentUI::slotUpdateFileList(void)
{
    SDriveEngine::inst()->getContentMngr()->get(SDriveEngine::inst()->getContentMngr()->getParentFolderUrl());
}

void ContentUI::setPanelDisplayingPath(const QString &name, EPath path, EPanels panel)
{
    QLabel* label = getPanelLabel(panel);
    QString pathDividerSign(QDir::toNativeSeparators("/"));
    int labelTextlength = label->text().length();
    int discStrLength = getDiscLength(panel);

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

int ContentUI::getDiscLength(EPanels panel) const
{
    SettingsManager settingsManager;
    QString disc(settingsManager.accountDisc(settingsManager.currentAccount(static_cast <int> (panel))));

    disc += QString(":");
    disc += QDir::toNativeSeparators("/");

    return disc.length();
}

QLabel* ContentUI::getPanelLabel(EPanels panel) const
{
    return  SDriveEngine::inst()->getFilePanel(panel)->getPathLabel();
}



#include <QLocale>
#include <QHeaderView>
#include "contentmanager.h"
#include "settings/settingsmanager.h"
#include "gui/forms/filepanel.h"

ContentManager::ContentManager(FilePanel *fp):
    panel(fp->getFileView()),
    panelNum(fp->panelNum),
    pathLabel(fp->getPathLabel()),
    accountsComboBox(fp->accountsComboBox)
{
}

ContentManager::~ContentManager()
{
    if(panel && !panel->topLevelItemCount() > 0) panel->clear();
}

void ContentManager::clear(void)
{
    treeWidgetItems.clear();

    if(treeWidgetItems.empty()) return;

    for(int i = 0; i < treeWidgetItems.count(); ++i)
    {
        delete treeWidgetItems[i];
    }
}

void ContentManager::show(void)
{
    if(!panel) return;

    clear();
    panel->clear();

    updateItemsState();
}

QString ContentManager::getDate(QDateTime &fileDateTime)
{
    return fileDateTime.toLocalTime().toString("ddd, MMM d yyyy, h:mm");
}

QString ContentManager::getSize(qint64 size)
{
    QString formattedSize;
    if(size > 0) formattedSize = QLocale().toString(size);
    return formattedSize;
}

void ContentManager::sectionClicked()
{
    SettingsManager().savePanelHeaderState(panelNum, panel->header()->saveState());
    show();
}

QTreeWidget* ContentManager::getPanel(void) const
{
    return panel;
}

bool ContentManager::hasItemParentSign(QTreeWidgetItem *item) const
{
    return item->data(0, Qt::DisplayRole).toString() == PARENT_FOLDER_SIGN;
}

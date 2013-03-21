#include <QLocale>
#include <QHeaderView>
#include "contentmanager.h"
#include "settings/settingsmanager.h"
#include "gui/forms/filepanel.h"
#include "share/debug.h"

ContentManager::ContentManager(FilePanel *fp):
    panel(fp->getFileView()),
    panelNum(fp->panelNum),
    pathLabel(fp->getPathLabel()),
    drivesComboBox(fp->drivesComboBox)
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

    if(!isRoot())
    {
        treeWidgetItems.push_back(new QTreeWidgetItem(panel));
        treeWidgetItems.last()->setText(0, PARENT_FOLDER_SIGN);
    }

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

QTreeWidget* ContentManager::getPanel() const
{
    return panel;
}

bool ContentManager::hasItemParentSign(QTreeWidgetItem *item) const
{
    return item->data(0, Qt::DisplayRole).toString() == PARENT_FOLDER_SIGN;
}

void ContentManager::addPath(const QString &path)
{
    if(!pathes.contains(path)) pathes.push_back(path);
}

QString ContentManager::back()
{
    QString backLink;

    if(!pathes.isEmpty()) pathes.pop_back();
    if(!pathes.isEmpty()) backLink = pathes.last();

    return backLink;
}

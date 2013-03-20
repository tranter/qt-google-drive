#include "contentmanager.h"
#include "settings/settingsmanager.h"
//#include "share/debug.h"
#include <QLocale>
#include <QDateTime>
#include <QHeaderView>

ContentManager::ContentManager(QTreeWidget *p, int pn):
    panel(p),
    panelNum(pn)
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

QString ContentManager::getDate(const QString &date)
{
    QDateTime fileDateTime = QDateTime::fromString(date, Qt::ISODate);
    return fileDateTime.toLocalTime().toString("ddd, MMM d yyyy, h:mm");
}

QString ContentManager::getSize(const QString &size)
{
    QString formattedSize;
    if(size.toLongLong() > 0) formattedSize = QLocale().toString(size.toLongLong());
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



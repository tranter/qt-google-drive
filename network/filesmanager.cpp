#include "filesmanager.h"
#include "share/registration.h"
#include "core/driveengine.h"
#include "share/debug.h"
#include "settings/settingsmanager.h"
//#include <QtAlgorithms>

FilesManager::FilesManager(QObject *parent):
    ContentManager(parent),
    panel(NULL)
{
}

FilesManager::~FilesManager()
{
    DEBUG;

    if(panel && !panel->topLevelItemCount() > 0)
    {
        panel->clear();
    }

    pathesURLs.clear();
}

void FilesManager::show(void)
{
    items = parser->getXMLHandler()->getItemInfo()->getItems();

    //qSort(items.begin(), items.end());
    //qSort(items.begin(), items.end(), qGreater<ItemInfo::Data>());

    SettingsManager settingsManager;

    clear();
    panel->clear();

    if(getRequest().url() != GET_FULL_ROOT_CONTENT)
    {
        isRoot = false;

        treeWidgetItems.push_back(new QTreeWidgetItem(panel));
        treeWidgetItems.last()->setText(0, PARENT_FOLDER_SIGN);
    }
    else
    {
        isRoot = true;
    }

    for(int i = 1; i < items.count(); ++i)
    {
        addItem(items[i]);
    }

    //DEBUG << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << items.count();

    //    for(int i = 1; i < items.count(); ++i)
    //    {
    //        if(items[i].type == FOLDER_TYPE_STR)
    //        {
    //            addItem(items[i]);
    //        }
    //    }

    //    for(int i = 1; i < items.count(); ++i)
    //    {
    //        if(items[i].type == FILE_TYPE_STR)
    //        {
    //            addItem(items[i]);
    //        }
    //    }

    if(settingsManager.initialLoading())
    {
        emit signalFirstPanelIsLoaded();
    }

    QString url(getRequest().url().toString());

    if(!pathesURLs.contains(url))
    {
        pathesURLs.push_back(url);
    }

    //SUi::inst()->filesView->setSortingEnabled(true);
    //SUi::inst()->filesView->sortItems(0, Qt::AscendingOrder);
    //connect(panel, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slotItemClicked(QTreeWidgetItem*, int)));
}

//bool FilesManager::compare(const ItemInfo::Data &s1, const ItemInfo::Data &s2)
//{
//    return s1.name < s2.name;
//}

void FilesManager::addItem(const ItemInfo::Data &itemData)
{
    treeWidgetItems.push_back(new QTreeWidgetItem(panel));

    treeWidgetItems.last()->setText(0, itemData.name);
    treeWidgetItems.last()->setIcon(0, QPixmap(itemData.iconPath));
    treeWidgetItems.last()->setText(1, itemData.dataOwner);
    treeWidgetItems.last()->setText(2, itemData.fileUpdated);
    treeWidgetItems.last()->setText(3, itemData.fileSize);
}

void FilesManager::setPanel(QTreeWidget *p)
{
    panel = p;
}

QTreeWidget* FilesManager::getPanel(void) const
{
    return panel;
}

QString FilesManager::getUpperLevelFolderURL(void) const
{
    return pathesURLs.last();
}

ItemInfo::Data FilesManager::getUpperLevelFolderInfo(void) const
{
    return  items[0];
}


QString FilesManager::back(void)
{
    QString prevLink;

    if(!pathesURLs.isEmpty())
    {
        pathesURLs.pop_back();
        prevLink = pathesURLs.last();
        pathesURLs.pop_back();
    }

    return prevLink;
}

QStringList FilesManager::getPathesURLs(void) const
{
    return pathesURLs;
}

void FilesManager::setPathesURLs(const QStringList &pathesURLsStrList)
{
    pathesURLs = pathesURLsStrList;
}

void FilesManager::sort(int column, Qt::SortOrder order)
{
}

void FilesManager::deleteFile(const QString &url)
{
    opMngr->deleteFile(url);
}

void FilesManager::copyWebFile(const ItemInfo::Data &source, const QString &destFolder)
{
    opMngr->copyWebFile(source, destFolder);
}

void FilesManager::moveWebFile(const ItemInfo::Data &source, const QString &destFolder)
{
    opMngr->moveWebFile(source, destFolder);
}

void FilesManager::renameWebFile(const ItemInfo::Data &source, const QString &newName)
{
    opMngr->renameWebFile(source, newName);
}

void FilesManager::shareWebFile(const ItemInfo::Data &source)
{
    opMngr->shareWebFile(source);
}

ItemInfo::Data FilesManager::getCurrentFileInfo(void)
{    
    int index;

    if(isRoot)
    {
        index = panel->currentIndex().row() + 1;
    }
    else
    {
        index = panel->currentIndex().row();
    }

    if(index < 0) index = 0;

    return  items[index];
}

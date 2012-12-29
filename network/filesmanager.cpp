#include "filesmanager.h"
#include "core/driveengine.h"
#include "share/debug.h"
#include "settings/settingsmanager.h"

FilesManager::FilesManager(QObject *parent):
    ContentManager(parent),
    panel(NULL)
{
}

FilesManager::~FilesManager()
{
    if(panel && !panel->topLevelItemCount() > 0)
    {
        panel->clear();
    }

    pathesURLs.clear();
}

void FilesManager::show(void)
{
    SettingsManager settingsManager;

    clear();
    panel->clear();

    //setItems();
    setItems(ItemInfo::Data::ETypeName);

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

    for(int i = 0; i < normalizedItems.count(); ++i)
    {
        addItem(normalizedItems[i]);
    }

    if(settingsManager.initialLoading())
    {
        emit signalFirstPanelIsLoaded();
    }

    QString url(getRequest().url().toString());

    if(!pathesURLs.contains(url))
    {
        pathesURLs.push_back(url);
    }
}

void FilesManager::setItems(ItemInfo::Data::ESortOrder itemSortOrder, Qt::SortOrder sortOrder)
{
    normalizedItems = parser->getXMLHandler()->getItemInfo()->getItems();

    if(!normalizedItems.isEmpty())
    {
        rootData = normalizedItems.takeFirst();
    }

    parser->getXMLHandler()->getItemInfo()->sort(normalizedItems, itemSortOrder, sortOrder);
}

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
    return  rootData;
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

    if(isRoot) index = panel->currentIndex().row();
    else index = panel->currentIndex().row() - 1;

    if(index < 0) index = 0;

    return  normalizedItems[index];
}

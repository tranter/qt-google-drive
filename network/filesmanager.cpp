#include "filesmanager.h"
#include "share/registration.h"
#include "core/driveengine.h"
#include "share/debug.h"
#include <QSettings>
//#include <QtAlgorithms>
#include <QDebug>

FilesManager::FilesManager(QObject *parent):
    ContentManager(parent)
{
}

FilesManager::~FilesManager()
{
    DEBUG;

    panel->clear();
    pathLinks.clear();
}

void FilesManager::show(void)
{
    fileItems = parser->getXMLHandler()->getItemInfo()->getFileItems();

    //qSort(fileItems.begin(), fileItems.end());
    //qSort(fileItems.begin(), fileItems.end(), qGreater<ItemInfo::Data>());

    QSettings settings(COMPANY_NAME, APP_NAME);

    clear();
    panel->clear();

    if(getRequest().url() != GET_FULL_ROOT_CONTENT)
    {
        isRoot = false;

        items.push_back(new QTreeWidgetItem(panel));
        items.last()->setText(0, PARENT_FOLDER_SIGN);
    }
    else
    {
        isRoot = true;
    }

    for(int i = 1; i < fileItems.count(); ++i)
    {
        DEBUG << "-------------------->" << fileItems[i].type;
//        if(fileItems[i].fileType == FOLDER_TYPE_STR)
//        {
            addItem(fileItems[i]);
//        }
    }

//    for(int i = 1; i < fileItems.count(); ++i)
//    {
//        if(fileItems[i].fileType == FILE_TYPE_STR)
//        {
//            addItem(fileItems[i]);
//        }
//    }

    if(settings.value(INIT_LOAD).toBool())
    {
        emit signalFirstPanelIsLoaded();
    }

    QString url(getRequest().url().toString());

    if(!pathLinks.contains(url))
    {
        pathLinks.push_back(url);
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
    items.push_back(new QTreeWidgetItem(panel));

    items.last()->setText(0, itemData.name);
    items.last()->setIcon(0, QPixmap(itemData.iconPath));
    items.last()->setText(1, itemData.dataOwner);
    items.last()->setText(2, itemData.fileUpdated);
    items.last()->setText(3, itemData.fileSize);
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
    return pathLinks.last();
}

ItemInfo::Data FilesManager::getUpperLevelFolderInfo(void) const
{
    return  fileItems[0];
}


QString FilesManager::back(void)
{
    QString prevLink;

    if(!pathLinks.isEmpty())
    {
        pathLinks.pop_back();
        prevLink = pathLinks.last();
        pathLinks.pop_back();
    }

    return prevLink;
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

    return  fileItems[index];
}

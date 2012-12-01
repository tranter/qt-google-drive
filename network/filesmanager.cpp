#include "filesmanager.h"
#include "share/registration.h"
#include "core/driveengine.h"
#include "share/debug.h"
#include <QSettings>
#include <QDebug>

FilesManager::FilesManager(QObject *parent):
    ContentManager(FILE_TYPE, parent)
{
}

FilesManager::~FilesManager()
{
    panel->clear();
    pathLinks.clear();
}

void FilesManager::show(void)
{
    fileItems = parser->getXMLHandler()->getItemInfo()->getFileItems();
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
        items.push_back(new QTreeWidgetItem(panel));

        items.last()->setText(0, fileItems[i].name);
        items.last()->setIcon(0, QPixmap(fileItems[i].iconPath));
        items.last()->setText(1, fileItems[i].dataOwner);
        items.last()->setText(2, fileItems[i].fileUpdated);
        items.last()->setText(3, fileItems[i].fileSize);
    }

    if(settings.value(INIT_LOAD).toBool())
    {
        emit signalFirstPanelIsLoaded();
    }

    pathLinks.push_back(getRequest().url().toString());

    //    SUi::inst()->filesView->setSortingEnabled(true);
    //    SUi::inst()->filesView->sortItems(0, Qt::AscendingOrder);
    //connect(panel, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slotItemClicked(QTreeWidgetItem*, int)));
}

void FilesManager::setPanel(QTreeWidget *p)
{
    panel = p;
}

QTreeWidget* FilesManager::getPanel(void) const
{
    return panel;
}

QString FilesManager::getUpLevelFolderLink(void) const
{
    return pathLinks.last();
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

void FilesManager::copyWebFile(const QString &fileName, const QString &source, const QString &dest)
{
    opMngr->copyWebFile(fileName, source, dest);
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

    return  fileItems[index];
}

//QString FilesManager::getRootLink(void) const
//{
//    return  fileItems[0].self;
//}

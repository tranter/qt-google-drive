#include "filesmanager.h"
#include "core/driveengine.h"
#include "share/debug.h"
#include "settings/settingsmanager.h"
#include <QLocale>
#include <QDateTime>
#include <QPair>

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
    if(!panel) return;

    clear();
    panel->clear();

    updateItemsState();

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

    if(SettingsManager().initialLoading()) emit signalFirstPanelIsLoaded();

    QString url(getRequest().url().toString());

    if(!pathesURLs.contains(url)) pathesURLs.push_back(url);

    connect(panel->header(),SIGNAL(sectionClicked(int)), this, SLOT(slotSectionClicked(int)));
}

void FilesManager::setItems(Items::Data::ESortOrder itemSortOrder, Qt::SortOrder sortOrder)
{
    normalizedItems = parser->getXMLHandler()->getItemInfo()->getItems();

    if(!normalizedItems.isEmpty())
    {
        rootData = normalizedItems.takeFirst();
    }

    parser->getXMLHandler()->getItemInfo()->sort(normalizedItems, itemSortOrder, sortOrder);
}

void FilesManager::addItem(const Items::Data &itemData)
{
    treeWidgetItems.push_back(new QTreeWidgetItem(panel));

    treeWidgetItems.last()->setText(0, itemData.name);
    treeWidgetItems.last()->setIcon(0, QPixmap(itemData.iconPath));
    treeWidgetItems.last()->setText(1, itemData.owner);
    treeWidgetItems.last()->setText(2, getDate(itemData.dateTime));
    treeWidgetItems.last()->setText(3, getSize(itemData.fileSize));
}

QString FilesManager::getDate(const QString &date)
{
    QDateTime fileDateTime = QDateTime::fromString(date, Qt::ISODate);
    return fileDateTime.toLocalTime().toString("ddd, MMM d yyyy, h:mm");
}

QString FilesManager::getSize(const QString &size)
{
    QString formattedSize;

    if(size.toLongLong() > 0)
    {
        formattedSize = QLocale().toString(size.toLongLong());
    }

    return formattedSize;
}

void FilesManager::setPanel(QTreeWidget *p, int pn)
{
    panel = p;
    panelNum = pn;
}

QTreeWidget* FilesManager::getPanel(void) const
{
    return panel;
}

QString FilesManager::getUpperLevelFolderURL(void) const
{
    return pathesURLs.last();
}

Items::Data FilesManager::getUpperLevelFolderInfo(void) const
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

void FilesManager::copyWebFile(const Items::Data &source, const QString &destFolder)
{
    opMngr->copyWebFile(source, destFolder);
}

void FilesManager::moveWebFile(const Items::Data &source, const QString &destFolder)
{
    opMngr->moveWebFile(source, destFolder);
}

void FilesManager::renameWebFile(const Items::Data &source, const QString &newName)
{
    opMngr->renameWebFile(source, newName);
}

void FilesManager::shareWebFile(const Items::Data &source)
{
    opMngr->shareWebFile(source);
}

Items::Data FilesManager::getCurrentFileInfo(void)
{    
    int index;

    if(isRoot) index = panel->currentIndex().row();
    else index = panel->currentIndex().row() - 1;

    if(index < 0) index = 0;

    return  normalizedItems[index];
}

void FilesManager::slotSectionClicked(int logicalIndex)
{
    Q_UNUSED(logicalIndex);
    SettingsManager().savePanelHeaderState(panelNum, panel->header()->saveState());
    show();
}

void FilesManager::updateItemsState(void)
{
    Items::Data::ESortOrder itemSortOrder;
    Qt::SortOrder sortOrder;

    QByteArray values(SettingsManager().restorePanelHeaderState(panelNum));

    if(values.isEmpty())
    {
        itemSortOrder = Items::Data::ETypeName;
        sortOrder = Qt::AscendingOrder;

        panel->header()->setSortIndicator(0, sortOrder);
    }
    else
    {
        panel->header()->restoreState(values);

        itemSortOrder = static_cast<Items::Data::ESortOrder> (panel->header()->sortIndicatorSection());
        sortOrder =  static_cast<Qt::SortOrder> (panel->header()->sortIndicatorOrder());
    }

    setItems(itemSortOrder, sortOrder);
}

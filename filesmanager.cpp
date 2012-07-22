#include "filesmanager.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

FilesManager::FilesManager(QObject *parent):
    ContentManager(FILE_TYPE, parent)
{
}

FilesManager::~FilesManager()
{
    SUi::inst()->filesViewRight->clear();
}

void FilesManager::show(void)
{
    QList<ItemInfo::Data> fileItems = parser->getXMLHandler()->getItemInfo()->getFileItems();

    clear();
    SUi::inst()->filesViewRight->clear();

    for(int i = 1; i < fileItems.count(); ++i)
    {
        items.push_back(new QTreeWidgetItem(SUi::inst()->filesViewRight));
        items.last()->setText(0, fileItems[i].name);
        items.last()->setIcon(0, QPixmap(fileItems[i].iconPath));
        items.last()->setText(1, fileItems[i].dataOwner);
        items.last()->setText(2, fileItems[i].fileUpdated);
        items.last()->setText(3, fileItems[i].fileSize);
    }

//    SUi::inst()->filesView->setSortingEnabled(true);
//    SUi::inst()->filesView->sortItems(0, Qt::AscendingOrder);

    QApplication::restoreOverrideCursor();
}

void FilesManager::sort(int column, Qt::SortOrder order)
{
}

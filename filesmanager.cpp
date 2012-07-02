#include "filesmanager.h"
#include "mainwindow.h"

FilesManager::FilesManager(QObject *parent):
    ContentManager(FILE_TYPE, parent)
{
}

FilesManager::~FilesManager()
{
    SUi::Inst()->filesView->clear();
}

void FilesManager::show(void)
{
    QList<TreeItemInfo::Data> fileItems = parser->getXMLHandler()->getTreeItemInfo()->getFileItems();
    clear();
    SUi::Inst()->filesView->clear();

    for(int i = 1; i < fileItems.count(); ++i)
    {
        items.push_back(new QTreeWidgetItem(SUi::Inst()->filesView));
        items.last()->setText(0, fileItems[i].name);
        items.last()->setIcon(0, QPixmap(fileItems[i].iconPath));
        items.last()->setText(1, fileItems[i].dataOwner);
        items.last()->setText(2, fileItems[i].fileUpdated);
        items.last()->setText(3, fileItems[i].fileSize);
    }

    SUi::Inst()->filesView->setSortingEnabled(true);
    SUi::Inst()->filesView->sortItems(0, Qt::AscendingOrder);
}

void FilesManager::sort(int column, Qt::SortOrder order)
{
}

#include "filesmanager.h"
#include "mainwindow.h"
#include <QDebug>

FilesManager::FilesManager(QObject *parent):
    ContentManager(FILE_TYPE, parent)
{
}

void FilesManager::show(void)
{
    QList<TreeItemInfo::Data> fileItems = parser->getXMLHandler()->getTreeItemInfo()->getFileItems();

    for(int i = 0; i < items.count(); ++i) delete items[i];
    items.clear();

    for(int i = 1; i < fileItems.count(); ++i)
    {
        items.push_back(new QTreeWidgetItem(UiInstance::ui->filesView));
        items.last()->setText(0, fileItems[i].name);
        items.last()->setIcon(0, QPixmap(fileItems[i].iconPath));
        items.last()->setText(1, fileItems[i].dataOwner);
        items.last()->setText(2, fileItems[i].fileUpdated);
        items.last()->setText(3, fileItems[i].fileSize);
    }

//    UiInstance::ui->filesView->setSortingEnabled(true);
//    UiInstance::ui->filesView->sortItems(0, Qt::AscendingOrder);
}

void FilesManager::sort(int column, Qt::SortOrder order)
{
}

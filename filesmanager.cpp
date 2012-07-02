#include "filesmanager.h"
#include "mainwindow.h"
#include <QDebug>

FilesManager::FilesManager(QObject *parent):
    ContentManager(FILE_TYPE, parent)
{
    qDebug() << "FilesManager::FilesManager()";
}

FilesManager::~FilesManager()
{
    qDebug() << "FilesManager::~FilesManager()";
    UiInstance::Instance()->filesView->clear();
}

void FilesManager::show(void)
{
    qDebug() << "FilesManager::show";

    QList<TreeItemInfo::Data> fileItems = parser->getXMLHandler()->getTreeItemInfo()->getFileItems();

    qDebug() << "FilesManager::show items.count():" << QString::number(items.count());

    clear();

    UiInstance::Instance()->filesView->clear();

    for(int i = 1; i < fileItems.count(); ++i)
    {
        items.push_back(new QTreeWidgetItem(UiInstance::Instance()->filesView));
        items.last()->setText(0, fileItems[i].name);
        items.last()->setIcon(0, QPixmap(fileItems[i].iconPath));
        items.last()->setText(1, fileItems[i].dataOwner);
        items.last()->setText(2, fileItems[i].fileUpdated);
        items.last()->setText(3, fileItems[i].fileSize);
    }

    UiInstance::Instance()->filesView->setSortingEnabled(true);
    UiInstance::Instance()->filesView->sortItems(0, Qt::AscendingOrder);
}

void FilesManager::sort(int column, Qt::SortOrder order)
{
}

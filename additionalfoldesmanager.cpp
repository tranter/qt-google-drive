#include "additionalfoldesmanager.h"
#include "mainwindow.h"

AdditionalFoldersManager::AdditionalFoldersManager(QObject *parent) :
      //ContentManager(FILE_TYPE, parent)
     FilesManager(parent)
{
}

AdditionalFoldersManager::~AdditionalFoldersManager()
{
 for(int i = 0; i < additionalFolders.count(); ++i) delete additionalFolders[i];
 additionalFolders. clear();
}

void AdditionalFoldersManager::create(const QString& name, QString iconName)
{
  additionalFolders.push_back(new QTreeWidgetItem(UiInstance::ui->additionalFoldersView));
  additionalFolders.last()->setText(0, name);
  additionalFolders.last()->setIcon(0, QPixmap(iconName));
}

void AdditionalFoldersManager::clear(void)
{
    for(int i = 0; i < items.count(); ++i) delete items[i];
    items.clear();
}

//void AdditionalFoldersManager::show(void)
//{
//    QList<TreeItemInfo::Data> fileItems = parser->getXMLHandler()->getTreeItemInfo()->getFileItems();

//    for(int i = 0; i < items.count(); ++i) delete items[i];
//    items.clear();

//    for(int i = 1; i < fileItems.count(); ++i)
//    {
//        items.push_back(new QTreeWidgetItem(UiInstance::ui->filesView));
//        items.last()->setText(0, fileItems[i].name);
//        items.last()->setIcon(0, QPixmap(fileItems[i].iconPath));
//        items.last()->setText(1, fileItems[i].dataOwner);
//        items.last()->setText(2, fileItems[i].fileUpdated);
//        items.last()->setText(3, fileItems[i].fileSize);
//    }

////    UiInstance::ui->filesViewWidget->setSortingEnabled(true);
////    UiInstance::ui->filesViewWidget->sortItems(0, Qt::AscendingOrder);
//}

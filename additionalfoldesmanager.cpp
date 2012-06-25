#include "additionalfoldesmanager.h"
#include "mainwindow.h"

AdditionalFoldersManager::AdditionalFoldersManager(QObject *parent) :
     FilesManager(parent)
{
}

AdditionalFoldersManager::~AdditionalFoldersManager()
{
 for(int i = 0; i < additionalFolders.count(); ++i) delete additionalFolders[i];
 additionalFolders.clear();
}

void AdditionalFoldersManager::create(const QString& name, QString iconName)
{
  additionalFolders.push_back(new QTreeWidgetItem(UiInstance::ui->additionalFoldersView));
  additionalFolders.last()->setText(0, name);
  additionalFolders.last()->setIcon(0, QPixmap(iconName));
}

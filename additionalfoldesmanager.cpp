#include "additionalfoldesmanager.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

AdditionalFoldersManager::AdditionalFoldersManager(QObject *parent) :
     FilesManager(parent)
{
}

AdditionalFoldersManager::~AdditionalFoldersManager()
{
 for(int i = 0; i < aFolders.count(); ++i) delete aFolders[i];
 aFolders.clear();
}

void AdditionalFoldersManager::create(const QString& name, QString iconName)
{
  aFolders.push_back(new QTreeWidgetItem(SUi::inst()->additionalFoldersView));
  aFolders.last()->setText(0, name);
  aFolders.last()->setIcon(0, QPixmap(iconName));
}

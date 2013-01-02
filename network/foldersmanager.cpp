#include "foldersmanager.h"
#include "gui/forms/mainwindow.h"
#include "ui_mainwindow.h"
#include "share/debug.h"

FoldersManager::FoldersManager(QObject *parent) :
    ContentManager(parent)
{
}

void FoldersManager::show(void)
{
}

void FoldersManager::createFolder(const QString &folderUrl, const QString &name)
{
    SOperationsManager::inst()->createFolder(folderUrl, name);
}

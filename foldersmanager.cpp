#include "foldersmanager.h"
#include "gui/forms/mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

FoldersManager::FoldersManager(QObject *parent) :
    ContentManager(FOLDER_TYPE, parent)
{
}

void FoldersManager::show(void)
{
    qDebug() << "FoldersManager::show";

    clear();
    SUi::inst()->treeFoldersView->clear();

    QTreeWidgetItem* root = new QTreeWidgetItem(SUi::inst()->treeFoldersView);

    items.push_back(root);
    items.last()->setText(0, MAIN_TITLE);

    buildTree(ROOT_TAG, items.last());

    //    UiInstance::ui->folderViewWidget->setSortingEnabled(true);
    //    UiInstance::ui->folderViewWidget->sortItems(0, Qt::AscendingOrder);

    root->setExpanded(true);
}

void FoldersManager::buildTree(const QString& searchStr, QTreeWidgetItem* parent)
{
    QList<QString> names, iconPathes;
    QList<QString> selfs;
    QList<int> indexes;

    folderItems = parser->getXMLHandler()->getItemInfo()->getItems();

    int count = 0;

    for (int i = 0; i < folderItems.count(); ++i)
    {
        if(folderItems[i].parent == searchStr)
        {
            names.push_back(folderItems[i].name);
            iconPathes.push_back(folderItems[i].iconPath);
            selfs.push_back(folderItems[i].self);
            indexes.push_back(i);
            ++count;
        }
    }

    if(count > 0)
    {
        fillTree(names, iconPathes, parent, indexes);
        for (int i = 0; i < count; ++i) buildTree(selfs[i], parent->child(i));
    }
}

void FoldersManager::fillTree(QList<QString> names, QList<QString> iconPathes, QTreeWidgetItem* parent, QList<int> indexes)
{
    for (int i = 0; i < names.count(); ++i)
    {
        items.push_back(new QTreeWidgetItem(parent));
        items.last()->setText(0, names[i]);
        items.last()->setIcon(0, QPixmap(iconPathes[i]));
        parser->getXMLHandler()->getItemInfo()->setPointer(indexes[i], items.last());
    }
}

void FoldersManager::insertFolder(QString name, QTreeWidgetItem* parent)
{    
    items.push_back(new QTreeWidgetItem(parent));
    items.last()->setText(0, name);

    parent->setExpanded(true);
}

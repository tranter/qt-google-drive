#include "foldersmanager.h"
#include "mainwindow.h"
#include <QDebug>

FoldersManager::FoldersManager(QObject *parent) :
    ContentManager(FOLDER_TYPE,parent)
{
}

void FoldersManager::slotError(QNetworkReply::NetworkError error)
{
    qDebug() << "FoldersManager::slotError error:" << error;

    if(error == QNetworkReply::UnknownNetworkError)
       qDebug() << "\n*******************\nIf this error occur, please make sure that you have openssl installed (also you can try just copy libeay32.dll and ssleay32.dll files from Qt SDK QtCreator/bin folder into your folder where your program .exe file located (tested on non-static compilation only))\n*******************\n";

    if(error == QNetworkReply::AuthenticationRequiredError) emit signalAccessTokenRequired();
}

void FoldersManager::show(void)
{
    for(int i = 0; i < items.count(); ++i) delete items[i];
    items.clear();
    UiInstance::ui->folderViewWidget->clear();

    QTreeWidgetItem* root = new QTreeWidgetItem(UiInstance::ui->folderViewWidget);

    items.push_back(root);
    items.last()->setText(0, "My Drive");

    buildTree(ROOT_TAG, items.last());

    UiInstance::ui->folderViewWidget->setSortingEnabled(true);
    UiInstance::ui->folderViewWidget->sortItems(0, Qt::AscendingOrder);
    root->setExpanded(true);
}

void FoldersManager::buildTree(const QString& searchStr, QTreeWidgetItem* parent)
{
    QList<QString> names, iconPathes;
    QList<QString> selfs;
    QList<int> indexes;
    folderItems = parser->getXMLHandler()->getTreeItemInfo()->getItems();

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

        for (int i = 0; i < count; ++i)
        {
            buildTree(selfs[i], parent->child(i));
        }
    }
}

void FoldersManager::fillTree(QList<QString> names, QList<QString> iconPathes, QTreeWidgetItem* parent, QList<int> indexes)
{
    for (int i = 0; i < names.count(); ++i)
    {
        items.push_back(new QTreeWidgetItem(parent));
        items.last()->setText(0, names[i]);
        items.last()->setIcon(0, QPixmap(iconPathes[i]));
        parser->getXMLHandler()->getTreeItemInfo()->setPointer(indexes[i], items.last());
    }
}

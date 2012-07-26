#include "operationsui.h"
#include <QDebug>

OperationsUI::OperationsUI(QObject *parent) :
    QObject(parent),
    createFolderDialog(NULL)
{
}

void OperationsUI::del(QObject* object)
{
    qDebug() << "OperationsUI::del() objectName:" << object->objectName();

    if (object == SUi::inst()->treeFoldersView)
    {
        if(!SDriveEngine::inst()->elStates[EAFoldersViewFocused])
        {
            ItemInfo item = *SDriveEngine::inst()->foldersMngr->getParser()->getXMLHandler()->getItemInfo();

            if(item[SDriveEngine::inst()->foldersUI->getCurrFolderItemId()].parent != "")
            {
                connect(SDriveEngine::inst()->foldersMngr->getOpMngr(), SIGNAL(signalDelFinished()), this, SLOT(slotDelFinished()));
                SDriveEngine::inst()->foldersMngr->del(item[SDriveEngine::inst()->foldersUI->getCurrFolderItemId()].self);
                delItemInTree(item);
            }
        }
    }

    if (object == SDriveEngine::inst()->getCurrFilesMngr()->getPanel())
    {
        qDebug() << "object == SDriveEngine::inst()->getCurrFilesMngr()->getPanel():" << object->objectName();

        FilesManager* manager;

        if(SDriveEngine::inst()->elStates[EAFoldersViewFocused]) manager = SDriveEngine::inst()->aFoldersMngr.data();
        else manager = SDriveEngine::inst()->getCurrFilesMngr();

        QList<ItemInfo::Data> itemData = manager->getParser()->getXMLHandler()->getItemInfo()->getFileItems();

        connect(manager->getOpMngr(), SIGNAL(signalDelFinished()), this, SLOT(slotDelFinished()));

        qDebug() << "OperationsUI::del itemData[SDriveEngine::inst()->filesUI->getCurrFileItemId(manager)].self:" << itemData[SDriveEngine::inst()->filesUI->getCurrFileItemId(manager)].self;

        manager->del(itemData[SDriveEngine::inst()->filesUI->getCurrFileItemId(manager)].self);
    }
}

void OperationsUI::delItemInTree(ItemInfo item)
{
    QTreeWidgetItem *parent = item[SDriveEngine::inst()->foldersUI->getCurrFolderItemId()].pointer->parent();
    int index;

    if (parent)
    {
        index = parent->indexOfChild(SUi::inst()->treeFoldersView->currentItem());
        delete parent->takeChild(index);
    }
    else
    {
        index = SUi::inst()->treeFoldersView->indexOfTopLevelItem(SUi::inst()->treeFoldersView->currentItem());
        delete SUi::inst()->treeFoldersView->takeTopLevelItem(index);
    }
}

void OperationsUI::slotTriggeredDel()
{
    if(SDriveEngine::inst()->elStates[ETrashFocused]) return;
    if(SDriveEngine::inst()->elStates[EAFoldersViewFocused] && !SDriveEngine::inst()->elStates[ERightViewFocused]) return;

    QObject* object;

    if(SDriveEngine::inst()->elStates[EFoldersTreeViewFocused]) object = SUi::inst()->treeFoldersView;
    else object = SDriveEngine::inst()->getCurrFilesMngr()->getPanel();

    del(object);
}

void OperationsUI::slotDelFinished()
{
    qDebug() << "OperationsUI::slotDelFinished";

    if(SDriveEngine::inst()->elStates[EAFoldersViewFocused]) SDriveEngine::inst()->filesUI->slotAShowFiles(SDriveEngine::inst()->getFoldersUI()->currAFolderId);
    if(SDriveEngine::inst()->elStates[EFoldersTreeViewFocused]) SDriveEngine::inst()->filesUI->showFiles();

    SDriveEngine::inst()->filesUI.data()->slotUpdateFileList();
}

void OperationsUI::slotCreateFolder()
{
    createFolderDialog = new CreateFolderDialog(SDriveEngine::inst()->parent);

    connect(createFolderDialog, SIGNAL(signalAccept(const QString&)), this, SLOT(slotAcceptCreateFolder(const QString&)));
    connect(createFolderDialog, SIGNAL(signalReject()), this, SLOT(slotRejectCreateFolder()));
    connect(createFolderDialog, SIGNAL(signalFinished(int)), this, SLOT(slotFinishedCreateFolder(int)));

    createFolderDialog->exec();
}

void OperationsUI::slotAcceptCreateFolder(const QString& name)
{
    createFolder(name);
    delete createFolderDialog;
}

void OperationsUI::slotRejectCreateFolder()
{
    delete createFolderDialog;
}

void OperationsUI::slotFinishedCreateFolder(int result)
{
    delete createFolderDialog;
}

void OperationsUI::createFolder(const QString& name)
{   
    if(name == "" || name.contains(QRegExp("[/\\\".<>]")))
    {
        CommonTools::msg("Please enter a valid name");
        return;
    }

    ItemInfo item = *SDriveEngine::inst()->foldersMngr->getParser()->getXMLHandler()->getItemInfo();
    int itemIndex = SDriveEngine::inst()->foldersUI->getCurrFolderItemId();

    SDriveEngine::inst()->foldersMngr->createFolder(item[itemIndex].self, name);
    SDriveEngine::inst()->foldersMngr->insertFolder(name, item[itemIndex].pointer);
}

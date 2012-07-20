#include "operationsui.h"
#include <QDebug>

OperationsUI::OperationsUI(QObject *parent) :
    QObject(parent),
    createFolderDialog(NULL)
{
}

void OperationsUI::del(QObject* object)
{
    qDebug() << "OperationsUI::del()";

    if (object == SUi::inst()->foldersView)
    {
        if(!SDriveEngine::inst()->elStates[DriveEngine::EAddViewFocused])
        {
            ItemInfo item = *SDriveEngine::inst()->foldersMngr->getParser()->getXMLHandler()->getItemInfo();

            if(item[SDriveEngine::inst()->foldersUI->getCurrentFolderItemIndex()].parent != "")
            {
                connect(SDriveEngine::inst()->foldersMngr->getOpMngr(), SIGNAL(signalDelFinished()), this, SLOT(slotDelFinished()));
                SDriveEngine::inst()->foldersMngr->del(item[SDriveEngine::inst()->foldersUI->getCurrentFolderItemIndex()].self);
                delItemInTree(item);
            }
        }
    }

    if (object == SUi::inst()->filesView)
    {
        FilesManager* manager;

        if(SDriveEngine::inst()->elStates[DriveEngine::EAddViewFocused]) manager = SDriveEngine::inst()->addlFilesMngr.data();
        else manager = SDriveEngine::inst()->filesMngr.data();

        QList<ItemInfo::Data> itemData = manager->getParser()->getXMLHandler()->getItemInfo()->getFileItems();

        connect(manager->getOpMngr(), SIGNAL(signalDelFinished()), this, SLOT(slotDelFinished()));
        manager->del(itemData[SDriveEngine::inst()->filesUI->getCurrentFileItemIndex(manager)].self);
    }
}

void OperationsUI::delItemInTree(ItemInfo item)
{
    QTreeWidgetItem *parent = item[SDriveEngine::inst()->foldersUI->getCurrentFolderItemIndex()].pointer->parent();
    int index;

    if (parent)
    {
        index = parent->indexOfChild(SUi::inst()->foldersView->currentItem());
        delete parent->takeChild(index);
    }
    else
    {
        index = SUi::inst()->foldersView->indexOfTopLevelItem(SUi::inst()->foldersView->currentItem());
        delete SUi::inst()->foldersView->takeTopLevelItem(index);
    }
}

void OperationsUI::slotTriggeredDel()
{
    if(SDriveEngine::inst()->elStates[DriveEngine::ETrashFocused]) return;
    if(SDriveEngine::inst()->elStates[DriveEngine::EAddViewFocused] && !SDriveEngine::inst()->elStates[DriveEngine::EFilesViewFocused]) return;

    QObject* object;

    if(SDriveEngine::inst()->elStates[DriveEngine::EFolderViewFocused]) object = SUi::inst()->foldersView;
    else object = SUi::inst()->filesView;

    del(object);
}

void OperationsUI::slotDelFinished()
{
    if(SDriveEngine::inst()->elStates[DriveEngine::EAddViewFocused]) SDriveEngine::inst()->filesUI->slotAdditionalShowFiles(SDriveEngine::inst()->getFoldersUI()->currAddFolderId);
    else SDriveEngine::inst()->filesUI->showFiles();
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
    ItemInfo item = *SDriveEngine::inst()->foldersMngr->getParser()->getXMLHandler()->getItemInfo();
    int itemIndex = SDriveEngine::inst()->foldersUI->getCurrentFolderItemIndex();

    if(name == "" || name.contains(QRegExp("[/\\\".<>]")))
    {
        CommonTools::msg("Please enter valid name");
        return;
    }

    SDriveEngine::inst()->foldersMngr->createFolder(item[itemIndex].self, name);
    SDriveEngine::inst()->foldersMngr->insertFolder(name, item[itemIndex].pointer);
}

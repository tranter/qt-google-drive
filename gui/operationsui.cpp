#include "operationsui.h"
#include "share/debug.h"

OperationsUI::OperationsUI(QObject *parent) :
    QObject(parent),
    createFolderDialog(NULL)
{
}

void OperationsUI::del(QObject* object)
{
    if (object == SUi::inst()->treeFoldersView)
    {
        if(!SDriveEngine::inst()->elStates[EAFoldersViewFocused])
        {
            ItemInfo item = *SDriveEngine::inst()->foldersMngr->getParser()->getXMLHandler()->getItemInfo();

            if(item[SDriveEngine::inst()->foldersUI->getCurrFolderItemId()].parent != "")
            {
                SDriveEngine::inst()->getFilesMngr()->deleteFile(item[SDriveEngine::inst()->foldersUI->getCurrFolderItemId()].self);
                delItemInTree(item);
            }
        }
    }

    if (object == SDriveEngine::inst()->getFilesMngr()->getPanel())
    {
        FilesManager* manager;

        if(SDriveEngine::inst()->elStates[EAFoldersViewFocused])
        {
            manager = SDriveEngine::inst()->aFoldersMngr.data();
        }
        else
        {
            manager = SDriveEngine::inst()->getFilesMngr();
        }

        QList<ItemInfo::Data> itemData = manager->getParser()->getXMLHandler()->getItemInfo()->getFileItems();

        manager->deleteFile(itemData[SDriveEngine::inst()->filesUI->getCurrFileItemId(manager)].self);
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

void OperationsUI::slotTriggeredDel(void)
{
    if(SDriveEngine::inst()->elStates[ETrashFocused]) return;
    if(SDriveEngine::inst()->elStates[EAFoldersViewFocused] && !SDriveEngine::inst()->elStates[ERightViewFocused]) return;

    QObject* object;

    if(SDriveEngine::inst()->elStates[EFoldersTreeViewFocused])
    {
        object = SUi::inst()->treeFoldersView;
    }
    else
    {
        object = SDriveEngine::inst()->getFilesMngr()->getPanel();
    }

    del(object);
}

void OperationsUI::slotNewFolder(void)
{
    createFolderDialog = new CreateFolderDialog(SDriveEngine::inst()->parent);

    connect(createFolderDialog, SIGNAL(signalAccept(const QString&)), this, SLOT(slotAcceptCreateFolder(const QString&)));
    connect(createFolderDialog, SIGNAL(signalReject()), this, SLOT(slotRejectCreateFolder()));
    connect(createFolderDialog, SIGNAL(signalFinished(int)), this, SLOT(slotFinishedCreateFolder(int)));

    createFolderDialog->exec();
}

void OperationsUI::slotCopyWebFile(void)
{
    ItemInfo::Data source = SDriveEngine::inst()->getFilesMngr()->getCurrentFileInfo();
    SDriveEngine::inst()->getFilesMngr()->copyWebFile(source, SDriveEngine::inst()->getFilesMngr(true)->getUpLevelFolderLink());
}

void OperationsUI::slotMoveWebFile(void)
{
  ItemInfo::Data source = SDriveEngine::inst()->getFilesMngr()->getCurrentFileInfo();
  SDriveEngine::inst()->getFilesMngr()->moveWebFile(source, SDriveEngine::inst()->getFilesMngr(true)->getUpLevelFolderLink());
}

void OperationsUI::slotRenameWebFile(void)
{
 ItemInfo::Data source = SDriveEngine::inst()->getFilesMngr()->getCurrentFileInfo();
 SDriveEngine::inst()->getFilesMngr()->renameWebFile(source, "Cool.col");
}

void OperationsUI::slotAcceptCreateFolder(const QString &name)
{
    createFolder(name);
}

void OperationsUI::slotRejectCreateFolder(void)
{
    delete createFolderDialog;
}

void OperationsUI::slotFinishedCreateFolder(int result)
{
    Q_UNUSED(result);
    delete createFolderDialog;
}

void OperationsUI::createFolder(const QString &name)
{   
    if(name == "" || name.contains(QRegExp("[/.<>]")) || name.contains(QRegExp("\\\\")) || name.contains(QRegExp("\"")))
    {
        CommonTools::msg("Please enter a valid name");
        return;
    }

    SDriveEngine::inst()->foldersMngr->createFolder(SDriveEngine::inst()->getFilesMngr()->getUpLevelFolderLink(), name);

    delete createFolderDialog;
}

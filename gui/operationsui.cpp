#include "operationsui.h"
#include "share/debug.h"

OperationsUI::OperationsUI(QObject *parent) :
    QObject(parent),
    createFolderDialog(NULL)
{
}

void OperationsUI::del(QObject* object)
{
    DEBUG << "objectName:" << object->objectName();

    if (object == SUi::inst()->treeFoldersView)
    {
        if(!SDriveEngine::inst()->elStates[EAFoldersViewFocused])
        {
            ItemInfo item = *SDriveEngine::inst()->foldersMngr->getParser()->getXMLHandler()->getItemInfo();

            if(item[SDriveEngine::inst()->foldersUI->getCurrFolderItemId()].parent != "")
            {
                connect(SDriveEngine::inst()->foldersMngr->getOpMngr(), SIGNAL(signalDelFinished()), this, SLOT(slotDelFinished()));
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

        connect(manager->getOpMngr(), SIGNAL(signalDelFinished()), this, SLOT(slotDelFinished()));
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

void OperationsUI::slotDelFinished(void)
{
    if(SDriveEngine::inst()->elStates[EAFoldersViewFocused])
    {
        SDriveEngine::inst()->filesUI->slotAShowFiles(SDriveEngine::inst()->getFoldersUI()->currAFolderId);
    }

    if(SDriveEngine::inst()->elStates[EFoldersTreeViewFocused])
    {
        SDriveEngine::inst()->filesUI->showFiles();
    }

    SDriveEngine::inst()->filesUI.data()->slotUpdateFileList();
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
    ItemInfo::Data fileData = SDriveEngine::inst()->getFilesMngr()->getCurrentFileInfo();

    //    SDriveEngine::inst()->uploadFileMngr.reset(new UploadFileManager(SDriveEngine::inst()->parent));
    //    connect(SDriveEngine::inst()->uploadFileMngr.data(), SIGNAL(signalUpdateFileList()), SDriveEngine::inst()->filesUI.data(), SLOT(slotUpdateFileList()));

     SDriveEngine::inst()->getFilesMngr()->copyWebFile(fileData, SDriveEngine::inst()->getFilesMngr(true)->getUpLevelFolderLink());

     DEBUG << "name of copying file" << fileData.name;

    //QString uploadLink(SDriveEngine::inst()->getFilesMngr(true)->getUpLevelFolderLink() + "/?convert=false");
    // QString uploadLink("https://docs.google.com/feeds/default/private/full/folder%3A0B_pGaTf6anqmNnR1M3dZMnotYzQ/contents/?convert=false");

    //  SDriveEngine::inst()->uploadFileMngr->startUpload(uploadLink, "3A0B_pGaTf6anqmNXpkZEl2ckFCZjA");
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
    if(name == "" || name.contains(QRegExp("[/\\\".<>]")))
    {
        CommonTools::msg("Please enter a valid name");
        return;
    }

    //DEBUG << "SDriveEngine::inst()->foldersUI->getFolderID()" << SDriveEngine::inst()->foldersUI->getFolderID();

    SDriveEngine::inst()->foldersMngr->createFolder(SDriveEngine::inst()->getFilesMngr()->getUpLevelFolderLink(), name);
    //SDriveEngine::inst()->foldersMngr->insertTreeItemFolder(name, SDriveEngine::inst()->getCurrFilesMngr()->getPanel());

    delete createFolderDialog;
}

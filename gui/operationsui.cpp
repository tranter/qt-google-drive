#include "operationsui.h"
#include "share/debug.h"

OperationsUI::OperationsUI(QObject *parent) :
    QObject(parent),
    createFolderDialog(NULL)
{
}

void OperationsUI::del(void)
{    
    FilesManager* manager;

    manager = SDriveEngine::inst()->getFilesMngr();
    manager->deleteFile(SDriveEngine::inst()->getFilesMngr()->getCurrentFileInfo().self);

}

void OperationsUI::slotTriggeredDel(void)
{   
    del();
}

bool OperationsUI::operationPossible(void)
{
    bool is = false;

    int index = SDriveEngine::inst()->getFilesMngr()->getPanel()->currentIndex().row();

    if(index >= 0)
    {
        QString itemText = SDriveEngine::inst()->getFilesMngr()->getPanel()->currentItem()->text(0);

        if(itemText != PARENT_FOLDER_SIGN)
        {
            is = true;
        }
    }

    return is;
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
    if(!operationPossible())
    {
        CommonTools::msg(tr("No Files selected"));
        return;
    }

    Items::Data source = SDriveEngine::inst()->getFilesMngr()->getCurrentFileInfo();
    SDriveEngine::inst()->getFilesMngr()->copyWebFile(source, SDriveEngine::inst()->getFilesMngr(true)->getUpperLevelFolderURL());
}

void OperationsUI::slotMoveWebFile(void)
{
    if(!operationPossible())
    {
        CommonTools::msg(tr("No Files selected"));
        return;
    }

    Items::Data source = SDriveEngine::inst()->getFilesMngr()->getCurrentFileInfo();
    SDriveEngine::inst()->getFilesMngr()->moveWebFile(source, SDriveEngine::inst()->getFilesMngr(true)->getUpperLevelFolderURL());
}

void OperationsUI::slotRenameWebFile(void)
{
    if(!operationPossible())
    {
        CommonTools::msg(tr("No Files selected"));
        return;
    }

    QTreeWidgetItem *item = SDriveEngine::inst()->getFilesMngr()->getPanel()->currentItem();

    editingItemText = item->text(0);

    item->setFlags(item->flags() | Qt::ItemIsEditable);
    SDriveEngine::inst()->getFilesMngr()->getPanel()->editItem(item, 0);

    connect(SDriveEngine::inst()->getFilesMngr()->getPanel()->itemDelegate(), SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)), this, SLOT(slotItemEditDone()));
}

void OperationsUI::slotItemEditDone(void)
{
    QTreeWidgetItem *item = SDriveEngine::inst()->getFilesMngr()->getPanel()->currentItem();
    Items::Data source = SDriveEngine::inst()->getFilesMngr()->getCurrentFileInfo();

    QString itemTextAfterEditing = item->text(0);

    if(editingItemText != itemTextAfterEditing)
    {
        SDriveEngine::inst()->getFilesMngr()->renameWebFile(source, itemTextAfterEditing);
        editingItemText.clear();
    }
}

void OperationsUI::slotShareWebFile(void)
{
    Items::Data source = SDriveEngine::inst()->getFilesMngr()->getCurrentFileInfo();
    SDriveEngine::inst()->getFilesMngr()->shareWebFile(source);
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
        CommonTools::msg(tr("Please enter a valid name"));
        return;
    }

    SDriveEngine::inst()->foldersMngr->createFolder(SDriveEngine::inst()->getFilesMngr()->getUpperLevelFolderURL(), name);
}

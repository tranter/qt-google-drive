#include "operationsmanager.h"
#include "share/commontools.h"
#include "core/driveengine.h"
#include "share/debug.h"
#include "gui/forms/sharedialog.h"
#include "settings/settingsmanager.h"
#include "share/enums.h"
#include <QStringList>

OperationsManager::OperationsManager(QObject *parent):
    NetworkManager(parent),
    currentOperation(ENone)
{
}

void OperationsManager::shareWebFile(const Items::Data &source)
{
    CommonTools::msg("Not Implemented yet");
    //      ShareDialog *shareDialog = new ShareDialog(SDriveEngine::inst()->getParent());
    //      shareDialog->show();
}

void OperationsManager::updatePanelContent(bool opposite)
{
    ContentManager* cm(SDriveEngine::inst()->getContentMngr(opposite));
    cm->get(cm->getParentFolderUrl());
}

void OperationsManager::setAccountInfo(const QString &accessToken, const QString &refreshToken)
{
    QString userInfoQuery, aboutInfoQuery;
    queries.userAboutInfo(userInfoQuery, aboutInfoQuery);

    accountInfo = new AccountInfo(userInfoQuery, aboutInfoQuery, accessToken, refreshToken);

    connect(accountInfo, SIGNAL(signalAccountInfoReceived(AccountInfo::Data&)), this, SLOT(slotAccountInfoReceived(AccountInfo::Data&)));

    accountInfo->setInfo();
}

void OperationsManager::slotAccountInfoReceived(AccountInfo::Data &data)
{
    SettingsManager settingsManager;

    if(!settingsManager.isAnyAccount())
    {
        settingsManager.setCurrentAccount(static_cast<int> (ELeft), data.email);
        settingsManager.setCurrentAccount(static_cast<int> (ERight), data.email);
    }

    settingsManager.saveAccountInfo(data);

    accountInfo->deleteLater();
    emit signalAccountInfoReadyToUse();
}

bool OperationsManager::operationPossible(void)
{
    bool is = false;

    int index = SDriveEngine::inst()->getContentMngr()->getPanel()->currentIndex().row();

    if(index >= 0)
    {
        QString itemText = SDriveEngine::inst()->getContentMngr()->getPanel()->currentItem()->text(0);

        if(itemText != PARENT_FOLDER_SIGN)
        {
            is = true;
        }
    }

    return is;
}

void OperationsManager::slotNewFolder(void)
{
    createFolderDialog = new CreateFolderDialog(SDriveEngine::inst()->getParent());

    connect(createFolderDialog, SIGNAL(signalAccept(const QString&)), this, SLOT(slotAcceptCreateFolder(const QString&)));
    connect(createFolderDialog, SIGNAL(signalReject()), this, SLOT(slotRejectCreateFolder()));
    connect(createFolderDialog, SIGNAL(signalFinished(int)), this, SLOT(slotFinishedCreateFolder(int)));

    createFolderDialog->setNameLabel(tr("New folder name:"));

    createFolderDialog->exec();
}

void OperationsManager::slotRejectCreateFolder(void)
{
    delete createFolderDialog;
}

void OperationsManager::slotFinishedCreateFolder(int result)
{
    Q_UNUSED(result);
    delete createFolderDialog;
}

void OperationsManager::slotCopyWebFile(void)
{
    if(!operationPossible())
    {
        CommonTools::msg(tr("No Files selected"));
        return;
    }

    Items::Data source(SDriveEngine::inst()->getContentMngr()->getCurrentFileInfo());
    QString destFolderUrl(SDriveEngine::inst()->getContentMngr(true)->getParentFolderUrl());

    copy.file(source, destFolderUrl);
}

void OperationsManager::slotMoveWebFile(void)
{
    if(!operationPossible())
    {
        CommonTools::msg(tr("No Files selected"));
        return;
    }

    Items::Data source(SDriveEngine::inst()->getContentMngr()->getCurrentFileInfo());
    QString destFolderUrl(SDriveEngine::inst()->getContentMngr(true)->getParentFolderUrl());

    move.file(source, destFolderUrl);
}

void OperationsManager::slotDelete(void)
{
    del.file(SDriveEngine::inst()->getContentMngr()->getCurrentFileInfo());
}

void OperationsManager::slotRenameWebFile(void)
{
    if(!operationPossible())
    {
        CommonTools::msg(tr("No Files selected"));
        return;
    }

    QTreeWidgetItem *item = SDriveEngine::inst()->getContentMngr()->getPanel()->currentItem();

    editingItemText = item->text(0);

    item->setFlags(item->flags() | Qt::ItemIsEditable);
    SDriveEngine::inst()->getContentMngr()->getPanel()->editItem(item, 0);

    connect(SDriveEngine::inst()->getContentMngr()->getPanel()->itemDelegate(), SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)), this, SLOT(slotItemEditDone()));
}

void OperationsManager::slotItemEditDone(void)
{
    QTreeWidgetItem *item = SDriveEngine::inst()->getContentMngr()->getPanel()->currentItem();
    Items::Data source = SDriveEngine::inst()->getContentMngr()->getCurrentFileInfo();

    QString itemTextAfterEditing = item->text(0);

    if(editingItemText != itemTextAfterEditing)
    {
        rename.file(source, itemTextAfterEditing);
        editingItemText.clear();
    }
}

void OperationsManager::slotShareWebFile(void)
{
    shareWebFile(SDriveEngine::inst()->getContentMngr()->getCurrentFileInfo());
}

void OperationsManager::slotAcceptCreateFolder(const QString &name)
{
    if(name == "" || name.contains(QRegExp("[/.<>]")) || name.contains(QRegExp("\\\\")) || name.contains(QRegExp("\"")))
    {
        CommonTools::msg(tr("Please enter a valid name"));
        return;
    }

//    if(operationPossible()) createFolder(name, SDriveEngine::inst()->getFilesMngr()->getParentFolderUrl());
//    else CommonTools::msg(tr("Please select a panel"));

    QString parentFolderUrl(SDriveEngine::inst()->getContentMngr()->getParentFolderUrl());

    create.folder(name, parentFolderUrl);
    delete createFolderDialog;
}






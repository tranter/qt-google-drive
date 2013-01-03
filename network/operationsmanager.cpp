#include "network/operationsmanager.h"
#include "share/commontools.h"
#include "core/driveengine.h"
#include "share/debug.h"
#include "gui/forms/sharedialog.h"
#include "settings/settingsmanager.h"
#include "share/enums.h"
#include <QStringList>

OperationsManager::OperationsManager(QObject *parent):
    NetworkManager(parent),
    currentOperation(ENone),
    isMove(false)
{
}

void OperationsManager::slotDelete(void)
{
    deleteFile(SDriveEngine::inst()->getFilesMngr()->getCurrentFileInfo());
}

void OperationsManager::deleteFile(const Items::Data &source)
{
    currentOperation = EDelete;

    queries.setRawHeader(SettingsManager().accessToken(), request);
    deleteRequest(queries.constructDeleteWebFileUrl(source.self));
}

void OperationsManager::copyWebFile(const Items::Data &source, const QString &destFolderUrl)
{    
    currentOperation = ECopy;

    postData = queries.getCopyWebFileData(source.name, destFolderUrl);
    queries.setRawHeader(SettingsManager().accessToken(), request);
    postRequest(queries.constructCopyWebFileUrl(source.self));
}

void OperationsManager::moveWebFile(const Items::Data &source, const QString &destFolderUrl)
{
    isMove = true;

    copyWebFile(source, destFolderUrl);
    fileUrlToDeleteForMoveOperation = source;
}

void OperationsManager::renameWebFile(const Items::Data &source, const QString &newName)
{
    currentOperation = ERename;

    postData = queries.getRenameWebFileData(newName);
    queries.setRawHeader(SettingsManager().accessToken(), request);
    putRequest(QUrl(source.self));
}

void OperationsManager::shareWebFile(const Items::Data &source)
{
    CommonTools::msg("Not Implemented yet");
    //      ShareDialog *shareDialog = new ShareDialog(SDriveEngine::inst()->getParent());
    //      shareDialog->show();
}

void OperationsManager::createFolder(const QString &name, const QString &folderUrl)
{ 
    postData = queries.getCreateFolderData(name, folderUrl);

    queries.setRawHeader(SettingsManager().accessToken(), request);
    postRequest(queries.constructCreateFolderUrl());
}

void OperationsManager::slotReplyFinished(QNetworkReply*)
{
    if(currentOperation == EDelete)
    {
        updatePanelContent(false);
    }
}

void OperationsManager::slotPostFinished(QNetworkReply* reply)
{
    NetworkManager::slotPostFinished(reply);

    if(currentOperation == ECopy)
    {
        updatePanelContent(true);

        if(isMove)
        {
            deleteFile(fileUrlToDeleteForMoveOperation);
            isMove = false;
        }
    }

    if(currentOperation == ECreateFolder)
    {
        updatePanelContent(false);
    }
}

void OperationsManager::slotPutFinished(void)
{
    if(currentOperation == ERename)
    {
        updatePanelContent(false);
    }
}

void OperationsManager::updatePanelContent(bool opposite)
{
    FilesManager* fileManager(SDriveEngine::inst()->getFilesMngr(opposite));
    fileManager->get(fileManager->getParentFolderUrl());
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

void OperationsManager::slotNewFolder(void)
{
    createFolderDialog = new CreateFolderDialog(SDriveEngine::inst()->getParent());

    connect(createFolderDialog, SIGNAL(signalAccept(const QString&)), this, SLOT(slotAcceptCreateFolder(const QString&)));
    connect(createFolderDialog, SIGNAL(signalReject()), this, SLOT(slotRejectCreateFolder()));
    connect(createFolderDialog, SIGNAL(signalFinished(int)), this, SLOT(slotFinishedCreateFolder(int)));

    createFolderDialog->exec();
}

void OperationsManager::slotCopyWebFile(void)
{
    if(!operationPossible())
    {
        CommonTools::msg(tr("No Files selected"));
        return;
    }

    Items::Data source = SDriveEngine::inst()->getFilesMngr()->getCurrentFileInfo();
    SDriveEngine::inst()->getFilesMngr()->copyWebFile(source, SDriveEngine::inst()->getFilesMngr(true)->getParentFolderUrl());
}

void OperationsManager::slotMoveWebFile(void)
{
    if(!operationPossible())
    {
        CommonTools::msg(tr("No Files selected"));
        return;
    }

    Items::Data source = SDriveEngine::inst()->getFilesMngr()->getCurrentFileInfo();
    SDriveEngine::inst()->getFilesMngr()->moveWebFile(source, SDriveEngine::inst()->getFilesMngr(true)->getParentFolderUrl());
}

void OperationsManager::slotRenameWebFile(void)
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

void OperationsManager::slotItemEditDone(void)
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

void OperationsManager::slotShareWebFile(void)
{
    Items::Data source = SDriveEngine::inst()->getFilesMngr()->getCurrentFileInfo();
    SDriveEngine::inst()->getFilesMngr()->shareWebFile(source);
}

void OperationsManager::slotAcceptCreateFolder(const QString &name)
{
    if(name == "" || name.contains(QRegExp("[/.<>]")) || name.contains(QRegExp("\\\\")) || name.contains(QRegExp("\"")))
    {
        CommonTools::msg(tr("Please enter a valid name"));
        return;
    }

    createFolder(name, SDriveEngine::inst()->getFilesMngr()->getParentFolderUrl());

    delete createFolderDialog;
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






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
    currentOperation = ECreateFolder;

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




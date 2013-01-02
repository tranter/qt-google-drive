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

void OperationsManager::deleteFile(const QString &sourceUrl)
{
    currentOperation = EDelete;

    CommonTools::setHeader(SettingsManager().accessToken(), request);
    request.setRawHeader("If-Match", "*");

    init();

    request.setUrl(getDeleteFileQuery(sourceUrl));

    reply = networkManager->deleteResource(request);

    connect(networkManager.data(), SIGNAL(finished(QNetworkReply*)),this, SLOT(slotReplyFinished(QNetworkReply*)));
    connectErrorHandlers();
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
    fileUrlToDeleteForMoveOperation = source.self;
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
//    currentOperation = ECreateFolder;

//    QString data = QString("{\"title\": \"%1\",\"parents\": [{\"id\": \"%2\"}],\"mimeType\": \"application/vnd.google-apps.folder\"}").arg(name).arg(CommonTools::getIDFromURL(folderUrl));

//    postData = data.toLatin1();

//    CommonTools::setHeader(SettingsManager().accessToken(), request);
//    request.setRawHeader("Content-Type", "application/json");
//    request.setRawHeader("Content-Length", QByteArray::number(postData.size()));

//    postRequest(QUrl("https://www.googleapis.com/drive/v2/files"));







    currentOperation = ECreateFolder;

    postData = queries.getCreateFolderData(name, folderUrl);

    queries.setRawHeader(SettingsManager().accessToken(), request);
    request.setRawHeader("Content-Length", QByteArray::number(postData.size()));

    postRequest(queries.constructCreateFolderUrl());
}

QUrl OperationsManager::getDeleteFileQuery(const QString &url)
{
    return QUrl(QString(DELETE_FILE + CommonTools::getIDFromURL(url)));
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
    FilesManager* fileManager = SDriveEngine::inst()->getFilesMngr(opposite);
    fileManager->get(fileManager->getUpperLevelFolderURL());
}

void OperationsManager::setAccountInfo(const QString &accessToken, const QString &refreshToken)
{
    QString userInfoQuery, aboutInfoQuery;
    queries.userAboutInfo(userInfoQuery, aboutInfoQuery);

    accountInfo = new AccountInfo(userInfoQuery, aboutInfoQuery, accessToken, refreshToken);

    connect(accountInfo, SIGNAL(signalAccountInfo(AccountInfo::Data&)), this, SLOT(slotAccountInfo(AccountInfo::Data&)));

    accountInfo->setInfo();
}

void OperationsManager::slotAccountInfo(AccountInfo::Data &data)
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




#include "filiestransfer.h"

FiliesTransfer::FiliesTransfer(QObject *parent) :
    QObject(parent)
{
}

//void FiliesTransfer::slotDownload(void)
//{
//    if(elementsStates[EAdditionalViewFocused]) download(additionalFilesManager.data());
//    else download(filesManager.data());
//}

//void FiliesTransfer::slotUpload(void)
//{
//    upload();
//}

void FiliesTransfer::download(FilesManager* manager)
{
    if(DriveEngineInstance::Instance()->downloadManager)
    {
        if(DriveEngineInstance::Instance()->downloadManager->getState() == NetworkManager::EBusy) return;
    }

    QSettings settings(COMPANY_NAME, APP_NAME);

    QList<TreeItemInfo::Data> item = manager->getParser()->getXMLHandler()->getTreeItemInfo()->getFileItems();
    int index = DriveEngineInstance::Instance()->getCurrentFileItemIndex(manager);

    QString downloadLink(item[index].downloadLink);

    if(!downloadLink.isEmpty())
    {
        if(DriveEngineInstance::Instance()->slotCheckWorkDir(false))
        {
            QString fileName = settings.value(WORK_DIR).toString() + "/" + item[index].name;
            QString fileType =  item[index].fileType;

            DriveEngineInstance::Instance()->downloadManager.reset(new DownloadFileManager(DriveEngineInstance::Instance()->parent));
            DriveEngineInstance::Instance()->downloadManager->startDownload(QUrl(downloadLink), fileName, fileType);
        }
        else CommonTools::msg(SET_DIR_REMINDER_MSG);
    }
}

void FiliesTransfer::upload(void)
{
    if(DriveEngineInstance::Instance()->elementsStates[DriveEngine::EAdditionalViewFocused])
    {
        CommonTools::msg("To upload a file please select a folder in \"Drive\" view");
        return;
    }

    if(DriveEngineInstance::Instance()->uploadFileManager)
    {
        if(DriveEngineInstance::Instance()->uploadFileManager->getState() == NetworkManager::EBusy) return;
    }

    QString fileName = QFileDialog::getOpenFileName(DriveEngineInstance::Instance()->parent, "Uploading file", QDir::homePath(), "All files(*)");

    if(!fileName.isEmpty())
    {
        TreeItemInfo item = *DriveEngineInstance::Instance()->foldersManager->getParser()->getXMLHandler()->getTreeItemInfo();
        int index = DriveEngineInstance::Instance()->getCurrentFolderItemIndex();

        QString uploadLink(item[index].uploadLink + "/?convert=false");

        if(!uploadLink.isEmpty())
        {
            DriveEngineInstance::Instance()->uploadFileManager.reset(new UploadFileManager(DriveEngineInstance::Instance()->parent));
            //connect(DriveEngineInstance::Instance()->uploadFileManager.data(), SIGNAL(signalUpdateFileList()), UiInstance::ui->, SLOT(slotUpdateFileList()));
            DriveEngineInstance::Instance()->uploadFileManager->startUpload(uploadLink, fileName);
        }
    }
}

#include "filestransferui.h"

FilesTransferUI::FilesTransferUI(QObject *parent) :
    QObject(parent)
{
}

void FilesTransferUI::download(FilesManager* manager)
{
    if(SDriveEngine::inst()->downloadManager)
    {
        if(SDriveEngine::inst()->downloadManager->getState() == NetworkManager::EBusy) return;
    }

    QSettings settings(COMPANY_NAME, APP_NAME);

    QList<TreeItemInfo::Data> item = manager->getParser()->getXMLHandler()->getTreeItemInfo()->getFileItems();
    int index = SDriveEngine::inst()->filesUI->getCurrentFileItemIndex(manager);

    QString downloadLink(item[index].downloadLink);

    if(!downloadLink.isEmpty())
    {
        if(SDriveEngine::inst()->slotCheckWorkDir(false))
        {
            QString fileName = settings.value(WORK_DIR).toString() + "/" + item[index].name;
            QString fileType =  item[index].fileType;

            SDriveEngine::inst()->downloadManager.reset(new DownloadFileManager(SDriveEngine::inst()->parent));
            SDriveEngine::inst()->downloadManager->startDownload(QUrl(downloadLink), fileName, fileType);
        }
        else CommonTools::msg(SET_DIR_REMINDER_MSG);
    }
}

void FilesTransferUI::upload(void)
{
    if(SDriveEngine::inst()->elementsStates[DriveEngine::EAdditionalViewFocused])
    {
        CommonTools::msg("To upload a file please select a folder in \"Drive\" view");
        return;
    }

    if(SDriveEngine::inst()->uploadFileManager)
    {
        if(SDriveEngine::inst()->uploadFileManager->getState() == NetworkManager::EBusy) return;
    }

    QString fileName = QFileDialog::getOpenFileName(SDriveEngine::inst()->parent, "Uploading file", QDir::homePath(), "All files(*)");

    if(!fileName.isEmpty())
    {
        TreeItemInfo item = *SDriveEngine::inst()->foldersManager->getParser()->getXMLHandler()->getTreeItemInfo();
        int index = SDriveEngine::inst()->getCurrentFolderItemIndex();

        QString uploadLink(item[index].uploadLink + "/?convert=false");

        if(!uploadLink.isEmpty())
        {
            SDriveEngine::inst()->uploadFileManager.reset(new UploadFileManager(SDriveEngine::inst()->parent));
            connect(SDriveEngine::inst()->uploadFileManager.data(), SIGNAL(signalUpdateFileList()), SDriveEngine::inst()->filesUI.data(), SLOT(slotUpdateFileList()));
            SDriveEngine::inst()->uploadFileManager->startUpload(uploadLink, fileName);
        }
    }
}

void FilesTransferUI::slotDownload(void)
{
    if(SDriveEngine::inst()->elementsStates[DriveEngine::EAdditionalViewFocused]) download(SDriveEngine::inst()->additionalFilesManager.data());
    else download(SDriveEngine::inst()->filesManager.data());
}

void FilesTransferUI::slotUpload(void)
{
    upload();
}

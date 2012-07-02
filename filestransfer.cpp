#include "filestransfer.h"

FilesTransfer::FilesTransfer(QObject *parent) :
    QObject(parent)
{
}

void FilesTransfer::download(FilesManager* manager)
{
    if(SDriveEngine::Inst()->downloadManager)
    {
        if(SDriveEngine::Inst()->downloadManager->getState() == NetworkManager::EBusy) return;
    }

    QSettings settings(COMPANY_NAME, APP_NAME);

    QList<TreeItemInfo::Data> item = manager->getParser()->getXMLHandler()->getTreeItemInfo()->getFileItems();
    int index = SDriveEngine::Inst()->getCurrentFileItemIndex(manager);

    QString downloadLink(item[index].downloadLink);

    if(!downloadLink.isEmpty())
    {
        if(SDriveEngine::Inst()->slotCheckWorkDir(false))
        {
            QString fileName = settings.value(WORK_DIR).toString() + "/" + item[index].name;
            QString fileType =  item[index].fileType;

            SDriveEngine::Inst()->downloadManager.reset(new DownloadFileManager(SDriveEngine::Inst()->parent));
            SDriveEngine::Inst()->downloadManager->startDownload(QUrl(downloadLink), fileName, fileType);
        }
        else CommonTools::msg(SET_DIR_REMINDER_MSG);
    }
}

void FilesTransfer::upload(void)
{
    if(SDriveEngine::Inst()->elementsStates[DriveEngine::EAdditionalViewFocused])
    {
        CommonTools::msg("To upload a file please select a folder in \"Drive\" view");
        return;
    }

    if(SDriveEngine::Inst()->uploadFileManager)
    {
        if(SDriveEngine::Inst()->uploadFileManager->getState() == NetworkManager::EBusy) return;
    }

    QString fileName = QFileDialog::getOpenFileName(SDriveEngine::Inst()->parent, "Uploading file", QDir::homePath(), "All files(*)");

    if(!fileName.isEmpty())
    {
        TreeItemInfo item = *SDriveEngine::Inst()->foldersManager->getParser()->getXMLHandler()->getTreeItemInfo();
        int index = SDriveEngine::Inst()->getCurrentFolderItemIndex();

        QString uploadLink(item[index].uploadLink + "/?convert=false");

        if(!uploadLink.isEmpty())
        {
            SDriveEngine::Inst()->uploadFileManager.reset(new UploadFileManager(SDriveEngine::Inst()->parent));
            connect(SDriveEngine::Inst()->uploadFileManager.data(), SIGNAL(signalUpdateFileList()), SDriveEngine::Inst(), SLOT(slotUpdateFileList()));
            SDriveEngine::Inst()->uploadFileManager->startUpload(uploadLink, fileName);
        }
    }
}

void FilesTransfer::slotDownload(void)
{
    qDebug("FilesTransfer::slotDownload");
    if(SDriveEngine::Inst()->elementsStates[DriveEngine::EAdditionalViewFocused]) download(SDriveEngine::Inst()->additionalFilesManager.data());
    else download(SDriveEngine::Inst()->filesManager.data());
}

void FilesTransfer::slotUpload(void)
{
    qDebug("FilesTransfer::slotUpload");
    upload();
}

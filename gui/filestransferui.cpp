#include "filestransferui.h"
#include "share/registration.h"
#include "share/debug.h"
#include <QSettings>
#include <QFileDialog>
#include <QDir>

FilesTransferUI::FilesTransferUI(QObject *parent) :
    QObject(parent)
{
}

void FilesTransferUI::download(void)
{
    if(SDriveEngine::inst()->downloadMngr)
    {
        if(SDriveEngine::inst()->downloadMngr->getState() == NetworkManager::EBusy) return;
    }

    QSettings settings(COMPANY_NAME, APP_NAME);
    ItemInfo::Data item = SDriveEngine::inst()->getFilesMngr()->getCurrentFileInfo();

    QString downloadLink(item.downloadLink);

    if(!downloadLink.isEmpty())
    {
        if(SDriveEngine::inst()->checkUI->slotCheckWorkDir(false))
        {
            QString fileName(settings.value(WORK_DIR).toString() + QDir::toNativeSeparators("/") + item.name);

            SDriveEngine::inst()->downloadMngr.reset(new DownloadFileManager(SDriveEngine::inst()->parent));
            SDriveEngine::inst()->downloadMngr->startDownload(QUrl(downloadLink), fileName, item.fileType);
        }
        else
        {
            CommonTools::msg(SET_DIR_REMINDER_MSG);
        }
    }
}

void FilesTransferUI::upload(void)
{
    if(SDriveEngine::inst()->uploadFileMngr)
    {
        if(SDriveEngine::inst()->uploadFileMngr->getState() == NetworkManager::EBusy) return;
    }

    QString  fileName = QFileDialog::getOpenFileName(SDriveEngine::inst()->parent, "Uploading file", QDir::homePath(), "All files(*)");

    if(!fileName.isEmpty())
    {
        QString uploadLink(SDriveEngine::inst()->getFilesMngr()->getUpperLevelFolderInfo().uploadLink + QString("/?convert=false"));

        SDriveEngine::inst()->uploadFileMngr.reset(new UploadFileManager(SDriveEngine::inst()->parent));
        connect(SDriveEngine::inst()->uploadFileMngr.data(), SIGNAL(signalUpdateFileList()), SDriveEngine::inst()->filesUI.data(), SLOT(slotUpdateFileList()));
        SDriveEngine::inst()->uploadFileMngr->startUpload(uploadLink, fileName);
    }
}

void FilesTransferUI::slotDownload(void)
{
    download();
}

void FilesTransferUI::slotUpload(void)
{
    upload();
}

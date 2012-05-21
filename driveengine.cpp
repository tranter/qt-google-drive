#include "driveengine.h"
#include <QDebug>
#include <QSettings>
#include "AppRegData.h"
#include <QMessageBox>

DriveEngine::DriveEngine(QObject *parent) :
    QObject(parent),
    oAuth2(NULL),
    downloadManager(NULL),
    uploadFileManager(NULL),
    foldersManager(NULL),
    filesManager(NULL)
{
    this->parent = static_cast<QWidget*>(parent);
}

DriveEngine::~DriveEngine()
{
    if(downloadManager) delete downloadManager;
    if(uploadFileManager) delete uploadFileManager;
    if(foldersManager) delete foldersManager;
    if(filesManager) delete filesManager;
    if(oAuth2) delete oAuth2;
}

void DriveEngine::slotStartLogin()
{
    oAuth2->startLogin(false);
}

void DriveEngine::slotStartLoginFromMenu()
{
    oAuth2->startLogin(true);
}

void DriveEngine::init(void)
{
    if(oAuth2) delete oAuth2;
    oAuth2 = new OAuth2(parent);

    connect(UiInstance::ui->folderViewWidget, SIGNAL(clicked (const QModelIndex&)), this, SLOT(slotFolderTreeViewClicked(const QModelIndex&)));

    showFolders();

    UiInstance::ui->filesViewWidget->header()->resizeSection(0, 400);
}

OAuth2* DriveEngine::getOAuth2(void) const
{
    return oAuth2;
}

void DriveEngine::slotDownload(void)
{
    if(downloadManager)
    {
        if(downloadManager->getState() == NetworkManager::EBusy) return;
    }

    QSettings settings(COMPANY_NAME, APP_NAME);

    QList<TreeItemInfo::Data> treeItems = filesManager->getParser()->getXMLHandler()->getTreeItemInfo()->getFileItems();
    int index = getCurrentFileItemIndex();

    QString downloadLink(treeItems[index].downloadLink);

    if(!downloadLink.isEmpty())
    {
        if(slotCheckWorkDir(false))
        {
            QString fileName = settings.value(WORK_DIR).toString() + "\/" + treeItems[index].name;
            QString fileType =  treeItems[index].fileType;

            if(downloadManager) delete downloadManager;
            downloadManager = new DownloadFileManager(parent);

            downloadManager->startDownload(QUrl(downloadLink), fileName, fileType);
        }
        else CommonTools::msg(SET_DIR_REMINDER_MSG);
    }
}

void DriveEngine::slotUpload(void)
{
    if(uploadFileManager)
    {
        if(uploadFileManager->getState() == NetworkManager::EBusy) return;
    }

    QString fileName = QFileDialog::getOpenFileName(parent, "Uploading file", QDir::homePath(), "All files(*)");

    if(!fileName.isEmpty())
    {
        TreeItemInfo treeItems = *foldersManager->getParser()->getXMLHandler()->getTreeItemInfo();
        int index = getCurrentModelItemIndex();

        QString uploadLink(treeItems[index].uploadLink + "/?convert=false");

        if(!uploadLink.isEmpty())
        {
            if(uploadFileManager) delete uploadFileManager;
            uploadFileManager = new UploadFileManager(parent);

            connect(uploadFileManager, SIGNAL(signalUpdateFileList()), parent, SLOT(slotUpdateFileList()));

            uploadFileManager->startUpload(uploadLink, fileName);
        }
    }
}

int DriveEngine::getCurrentModelItemIndex(void) const
{
    QTreeWidgetItem* pointer = static_cast<QTreeWidgetItem*>(UiInstance::ui->folderViewWidget->currentIndex().internalPointer());
    TreeItemInfo treeItems = *foldersManager->getParser()->getXMLHandler()->getTreeItemInfo();
    int count = treeItems.getItems().count();

    int currentModelIndex = 0;

    for(int i = 0; i < count; ++i)
    {
        if(treeItems[i].pointer == pointer)
        {
            currentModelIndex = i;
            break;
        }
    }

    return currentModelIndex;
}

int DriveEngine::getCurrentFileItemIndex(void) const
{
    QList<TreeItemInfo::Data> fileItemsInfo = filesManager->getParser()->getXMLHandler()->getTreeItemInfo()->getFileItems();
    int count = fileItemsInfo.count();
    QString fileName(UiInstance::ui->filesViewWidget->currentIndex().data().toString());

    int currentFileIndex = 0;

    for(int i = 1; i < count; ++i)
    {
        if(fileName == fileItemsInfo[i].name)
        {
            currentFileIndex = i;
            break;
        }
    }

    return currentFileIndex;
}

bool DriveEngine::slotCheckWorkDir(bool showDlg)
{
    QSettings settings(COMPANY_NAME, APP_NAME);
    SettingsDialog dlg(parent);
    bool dirTextNotEmpty = false;

    if(settings.contains(WORK_DIR) && showDlg)
    {
        dlg.setDirectoryPath(settings.value(WORK_DIR).toString());
    }
    else if(settings.contains(WORK_DIR) && !showDlg)
    {
        return true;
    }

    switch(dlg.exec())
    {
    case QDialog::Accepted:
    {
        if(!dlg.directoryPath().isEmpty() )
        {
            settings.setValue(WORK_DIR,dlg.directoryPath());
            dirTextNotEmpty = true;
        }

    }
        break;
    }

    return dirTextNotEmpty;
}

void DriveEngine::slotFolderTreeViewClicked(const QModelIndex& index)
{
    showFiles();
}

void DriveEngine::showFolders(void)
{
    if(!foldersManager) foldersManager = new FoldersManager;
    foldersManager->getFolders(GET_FOLDERS);
}

void DriveEngine::showFiles(void)
{
    TreeItemInfo treeItems = *foldersManager->getParser()->getXMLHandler()->getTreeItemInfo();
    int treeItemsIndex = getCurrentModelItemIndex();

    if(treeItems[treeItemsIndex].type == FOLDER_TYPE_STR)
    {
        if(!filesManager) filesManager = new FilesManager;

        QString query(treeItems[treeItemsIndex].self);
        query += QString("/contents?max-results=10000");

        qDebug() << "query:" << query;

        filesManager->getFiles(query);
    }
}

FoldersManager* DriveEngine::getFoldersManager(void) const
{
    return foldersManager;
}

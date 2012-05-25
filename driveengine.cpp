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
    filesManager(NULL),
    additionalFilesManager(NULL)
{
    this->parent = static_cast<QWidget*>(parent);
    for(int i = 0; i < EElementsStatesCount; ++i) elementsStates[i] = false;
    qDebug() << "DriveEngine::DriveEngine()";
}

DriveEngine::~DriveEngine()
{
    qDebug() << "DriveEngine::~DriveEngine()";

    if(downloadManager) downloadManager->deleteLater();
    if(uploadFileManager) uploadFileManager->deleteLater();
    if(foldersManager) foldersManager->deleteLater();
    if(filesManager) filesManager->deleteLater();
    if(additionalFilesManager) additionalFilesManager->deleteLater();
    if(oAuth2) oAuth2->deleteLater();
}

void DriveEngine::slotStartLogin()
{
    qDebug() << "DriveEngine::slotStartLogin";
    oAuth2->startLogin(false);
}

void DriveEngine::slotStartLoginFromMenu()
{
    oAuth2->startLogin(true);
}

void DriveEngine::init(void)
{
    if(!oAuth2) oAuth2 = new OAuth2(parent);
    //if(!foldersManager) foldersManager = new FoldersManager;
    if(!additionalFilesManager) additionalFilesManager = new AdditionalFoldersManager;
    if(!filesManager) filesManager = new FilesManager;

    setConnections();
    showFolders();
    showAdditionalFolders();

    UiInstance::ui->filesView->header()->resizeSection(0, 380);
}

void DriveEngine::setConnections(void)
{
    connect(UiInstance::ui->foldersView, SIGNAL(clicked (const QModelIndex&)), this, SLOT(slotFoldersViewClicked(const QModelIndex&)));
    connect(UiInstance::ui->filesView, SIGNAL(clicked (const QModelIndex&)), this, SLOT(slotFilesViewClicked(const QModelIndex&)));
    connect(UiInstance::ui->additionalFoldersView, SIGNAL(clicked (const QModelIndex&)), this, SLOT(slotAdditionalShowFiles(const QModelIndex&)));
    connect(UiInstance::ui->filesView->header(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), this, SLOT(slotFilesSortIndicatorChanged(int, Qt::SortOrder)));
}

OAuth2* DriveEngine::getOAuth2(void) const
{
    return oAuth2;
}

void DriveEngine::slotDownload(void)
{    
    if(elementsStates[EAdditionalViewFocused]) download(additionalFilesManager);
    else download(filesManager);
}

void DriveEngine::slotUpload(void)
{
    upload();
}

void DriveEngine::download(FilesManager* manager)
{
    if(downloadManager)
    {
        if(downloadManager->getState() == NetworkManager::EBusy) return;
    }

    QSettings settings(COMPANY_NAME, APP_NAME);

    QList<TreeItemInfo::Data> treeItems = manager->getParser()->getXMLHandler()->getTreeItemInfo()->getFileItems();
    int index = getCurrentFileItemIndex(manager);

    QString downloadLink(treeItems[index].downloadLink);

    if(!downloadLink.isEmpty())
    {
        if(slotCheckWorkDir(false))
        {
            QString fileName = settings.value(WORK_DIR).toString() + "\/" + treeItems[index].name;
            QString fileType =  treeItems[index].fileType;

            if(downloadManager) downloadManager->deleteLater();
            downloadManager = new DownloadFileManager(parent);

            downloadManager->startDownload(QUrl(downloadLink), fileName, fileType);
        }
        else CommonTools::msg(SET_DIR_REMINDER_MSG);
    }
}

void DriveEngine::upload(void)
{
    if(elementsStates[EAdditionalViewFocused])
    {
        CommonTools::msg("To upload a file please select a folder in \"Drive\" view");
        return;
    }

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
            if(uploadFileManager) uploadFileManager->deleteLater();
            uploadFileManager = new UploadFileManager(parent);

            connect(uploadFileManager, SIGNAL(signalUpdateFileList()), parent, SLOT(slotUpdateFileList()));

            uploadFileManager->startUpload(uploadLink, fileName);
        }
    }
}

int DriveEngine::getCurrentModelItemIndex(void) const
{
    QTreeWidgetItem* pointer = static_cast<QTreeWidgetItem*>(UiInstance::ui->foldersView->currentIndex().internalPointer());
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

int DriveEngine::getCurrentFileItemIndex(FilesManager* manager) const
{
    QList<TreeItemInfo::Data> fileItemsInfo = manager->getParser()->getXMLHandler()->getTreeItemInfo()->getFileItems();
    int count = fileItemsInfo.count();
    QString fileName(UiInstance::ui->filesView->currentIndex().data().toString());

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

void DriveEngine::slotFoldersViewClicked(const QModelIndex& index)
{
    qDebug()  << "DriveEngine::slotFoldersViewClicked index" << QString::number(index.row());

    currentFolderIndex = index.row();

    elementsStates[EFolderViewFocused] = true;
    elementsStates[EAdditionalViewFocused] = false;
    elementsStates[EFilesViewFocused] = false;
    elementsStates[ETrashFocused] = false;

    additionalFilesManager->clear();
    showFiles();
}

void DriveEngine::slotFilesViewClicked(const QModelIndex&)
{
    qDebug()  << "DriveEngine::slotFilesViewClicked";
    elementsStates[EFolderViewFocused] = false;
    elementsStates[EFilesViewFocused] = true;
    if(!elementsStates[EAdditionalViewFocused]) showFilesFromFolderInFilesView();
}

void DriveEngine::slotAdditionalShowFiles(const QModelIndex& index)
{
    qDebug()  << "DriveEngine::slotAdditionalFoldersView" << index.model()->data(index).toString();

    if(index.model()->data(index).toString() == TRASH_TITLE) elementsStates[ETrashFocused] = true;
    else elementsStates[ETrashFocused] = false;

    QString query;
    currentAdditionalFolderIndex = index;

    elementsStates[EFolderViewFocused] = false;
    elementsStates[EAdditionalViewFocused] = true;
    elementsStates[EFilesViewFocused] = false;

    filesManager->clear();

    if(index.model()->data(index).toString() == ALL_ITEMS_TITLE) query = GET_ALL_ITEMS + MAX_RESULTS;
    if(index.model()->data(index).toString() == GET_USER_DOCUMENTS_TITLE) query = GET_USER_DOCUMENTS + MAX_RESULTS;
    if(index.model()->data(index).toString() == GET_USER_PRESENTATIONS_TITLE) query = GET_USER_PRESENTATIONS + MAX_RESULTS;
    if(index.model()->data(index).toString() == GET_USER_SPREADSHEETS_TITLE) query = GET_USER_SPREADSHEETS + MAX_RESULTS;
    if(index.model()->data(index).toString() == OWNED_BY_ME_TITLE) query = GET_OWNED_BY_ME + MAX_RESULTS;
    if(index.model()->data(index).toString() == GET_STARRED_TITLE)  query = GET_STARRED;
    if(index.model()->data(index).toString() == TRASH_TITLE) query = GET_TRASH;

    additionalFilesManager->get(query);
}

void DriveEngine::showFolders(void)
{
    if(foldersManager) delete foldersManager;
    foldersManager = new FoldersManager;

    foldersManager->get(GET_FOLDERS);
    //connect(foldersManager, SIGNAL(signalFoldersShowed()), this, SLOT(slotFoldersShowed()));
}

void DriveEngine::showAdditionalFolders(void)
{
    QString generalImage(":ico/allitems");

    additionalFilesManager->create(ALL_ITEMS_TITLE, generalImage);
    additionalFilesManager->create(GET_USER_DOCUMENTS_TITLE, generalImage);
    additionalFilesManager->create(GET_USER_PRESENTATIONS_TITLE, generalImage);
    additionalFilesManager->create(GET_USER_SPREADSHEETS_TITLE, generalImage);
    additionalFilesManager->create(OWNED_BY_ME_TITLE, generalImage);
    additionalFilesManager->create(GET_STARRED_TITLE, generalImage);
    additionalFilesManager->create(TRASH_TITLE, ":ico/trash");
}

void DriveEngine::showFiles(void)
{
    qDebug()  << "DriveEngine::showFiles";

    TreeItemInfo treeItems = *foldersManager->getParser()->getXMLHandler()->getTreeItemInfo();
    int treeItemsIndex = getCurrentModelItemIndex();

    if(treeItems[treeItemsIndex].type == FOLDER_TYPE_STR)
    {
        QString query(treeItems[treeItemsIndex].self);
        query += QString(CONTENTS + MAX_RESULTS);

        qDebug() << "query:" << query;

        filesManager->get(query);
    }
}

bool DriveEngine::folderInFilesView(QString& resourceID)
{
    QList<TreeItemInfo::Data> treeItems = filesManager->getParser()->getXMLHandler()->getTreeItemInfo()->getFileItems();
    int index = getCurrentFileItemIndex(filesManager);
    bool isFolder = false;

    QString str(treeItems[index].self);
    QStringList strList = str.split("/");
    str = strList[strList.count() - 1];

    if(str.indexOf(FOLDER_TYPE_STR) != -1)
    {
        resourceID = str;
        isFolder = true;
    }

    return isFolder;
}

void DriveEngine::showFilesFromFolderInFilesView(void)
{
    QString str;

    if(folderInFilesView(str))
    {
        QString query(GET_FILES_IN_FOLDER);
        query += str;
        query += QString(CONTENTS + MAX_RESULTS);

        qDebug() << "query:" << query;

        filesManager->get(query);
    }
}

FoldersManager* DriveEngine::getFoldersManager(void) const
{
    return foldersManager;
}

void DriveEngine::slotFilesSortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
{
    qDebug() << "index:" << QString::number(logicalIndex) << " order:" << order;
}

void DriveEngine::slotDel(QObject* object)
{
    qDebug() << "DriveEngine::slotDel";
    if (object == UiInstance::ui->foldersView)
    {
        qDebug() << "folder";
        if(!elementsStates[EAdditionalViewFocused])
        {
            TreeItemInfo item = *foldersManager->getParser()->getXMLHandler()->getTreeItemInfo();

            if(item[getCurrentModelItemIndex()].parent != "")
            {
                connect(foldersManager->getOperationsManager(), SIGNAL(signalDelFinished()), this, SLOT(slotDelFinished()));
                foldersManager->del(item[getCurrentModelItemIndex()].self);
                delItemInTree(item);
            }
        }
    }

    if (object == UiInstance::ui->filesView)
    {
        qDebug() << "file";
        FilesManager* manager;

        if(elementsStates[EAdditionalViewFocused]) manager = additionalFilesManager;
        else manager = filesManager;

        QList<TreeItemInfo::Data> itemData = manager->getParser()->getXMLHandler()->getTreeItemInfo()->getFileItems();

        connect(manager->getOperationsManager(), SIGNAL(signalDelFinished()), this, SLOT(slotDelFinished()));
        manager->del(itemData[getCurrentFileItemIndex(manager)].self);
    }
}

void DriveEngine::delItemInTree(TreeItemInfo item)
{
    QTreeWidgetItem *parent = item[getCurrentModelItemIndex()].pointer->parent();
    int index;

    if (parent)
    {
        index = parent->indexOfChild(UiInstance::ui->foldersView->currentItem());
        delete parent->takeChild(index);
    }
    else
    {
        index = UiInstance::ui->foldersView->indexOfTopLevelItem(UiInstance::ui->foldersView->currentItem());
        delete UiInstance::ui->foldersView->takeTopLevelItem(index);
    }
}

void DriveEngine::slotTriggeredDel()
{
    qDebug() << "DriveEngine::slotTriggeredDel";

    if(elementsStates[ETrashFocused]) return;
    if(elementsStates[EAdditionalViewFocused] && !elementsStates[EFilesViewFocused]) return;

    QObject* object;

    if(elementsStates[EFolderViewFocused]) object = UiInstance::ui->foldersView;
    else object = UiInstance::ui->filesView;

    slotDel(object);
}

void DriveEngine::slotDelFinished()
{
    qDebug() << "DriveEngine::slotDelFinished";

    //if(elementsStates[EFolderViewFocused]) return;

    if(elementsStates[EAdditionalViewFocused]) slotAdditionalShowFiles(currentAdditionalFolderIndex);
    else showFiles();
}

//void DriveEngine::slotFoldersShowed()
//{
//    qDebug() << "DriveEngine::slotFoldersShowed()";
//}

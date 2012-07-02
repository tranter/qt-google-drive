#include "driveengine.h"
#include <QDebug>
#include <QSettings>
#include "AppRegData.h"
#include <QMessageBox>

DriveEngine::DriveEngine(QObject *parent) :
    QObject(parent),
    createFolderDialog(NULL)
{
    this->parent = static_cast<QWidget*>(parent);
    for(int i = 0; i < EElementsStatesCount; ++i) elementsStates[i] = false;
}

DriveEngine::~DriveEngine()
{
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
    oAuth2.reset(new OAuth2(parent));
    additionalFilesManager.reset(new AdditionalFoldersManager);
    filesManager.reset(new FilesManager);
    filiesTransfer.reset(new FiliesTransfer);

    setConnections();
    showFolders();
    showAdditionalFolders();

    SUi::Inst()->filesView->header()->resizeSection(0, 380);
}

void DriveEngine::setConnections(void)
{
    connect(SUi::Inst()->foldersView, SIGNAL(clicked (const QModelIndex&)), this, SLOT(slotFoldersViewClicked(const QModelIndex&)));
    connect(SUi::Inst()->filesView, SIGNAL(clicked (const QModelIndex&)), this, SLOT(slotFilesViewClicked(const QModelIndex&)));
    connect(SUi::Inst()->additionalFoldersView, SIGNAL(clicked (const QModelIndex&)), this, SLOT(slotAdditionalShowFiles(const QModelIndex&)));
    connect(SUi::Inst()->filesView->header(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), this, SLOT(slotFilesSortIndicatorChanged(int, Qt::SortOrder)));
}

OAuth2* DriveEngine::getOAuth2(void) const
{
    return oAuth2.data();
}

void DriveEngine::slotDownload(void)
{
    if(elementsStates[EAdditionalViewFocused]) filiesTransfer->download(additionalFilesManager.data());
    else filiesTransfer->download(filesManager.data());
}

void DriveEngine::slotUpload(void)
{
    filiesTransfer->upload();
}

//void DriveEngine::download(FilesManager* manager)
//{
//    if(downloadManager)
//    {
//        if(downloadManager->getState() == NetworkManager::EBusy) return;
//    }

//    QSettings settings(COMPANY_NAME, APP_NAME);

//    QList<TreeItemInfo::Data> item = manager->getParser()->getXMLHandler()->getTreeItemInfo()->getFileItems();
//    int index = getCurrentFileItemIndex(manager);

//    QString downloadLink(item[index].downloadLink);

//    if(!downloadLink.isEmpty())
//    {
//        if(slotCheckWorkDir(false))
//        {
//            QString fileName = settings.value(WORK_DIR).toString() + "/" + item[index].name;
//            QString fileType =  item[index].fileType;

//            downloadManager.reset(new DownloadFileManager(parent));
//            downloadManager->startDownload(QUrl(downloadLink), fileName, fileType);
//        }
//        else CommonTools::msg(SET_DIR_REMINDER_MSG);
//    }
//}

//void DriveEngine::upload(void)
//{
//    if(elementsStates[EAdditionalViewFocused])
//    {
//        CommonTools::msg("To upload a file please select a folder in \"Drive\" view");
//        return;
//    }

//    if(uploadFileManager)
//    {
//        if(uploadFileManager->getState() == NetworkManager::EBusy) return;
//    }

//    QString fileName = QFileDialog::getOpenFileName(parent, "Uploading file", QDir::homePath(), "All files(*)");

//    if(!fileName.isEmpty())
//    {
//        TreeItemInfo item = *foldersManager->getParser()->getXMLHandler()->getTreeItemInfo();
//        int index = getCurrentFolderItemIndex();

//        QString uploadLink(item[index].uploadLink + "/?convert=false");

//        if(!uploadLink.isEmpty())
//        {
//            uploadFileManager.reset(new UploadFileManager(parent));
//            connect(uploadFileManager.data(), SIGNAL(signalUpdateFileList()), parent, SLOT(slotUpdateFileList()));
//            uploadFileManager->startUpload(uploadLink, fileName);
//        }
//    }
//}

int DriveEngine::getCurrentFolderItemIndex(void) const
{
    QTreeWidgetItem* pointer = static_cast<QTreeWidgetItem*>(SUi::Inst()->foldersView->currentIndex().internalPointer());
    TreeItemInfo item = *foldersManager->getParser()->getXMLHandler()->getTreeItemInfo();
    int count = item.getItems().count();

    int currentModelIndex = 0;

    for(int i = 0; i < count; ++i)
    {
        if(item[i].pointer == pointer)
        {
            currentModelIndex = i;
            break;
        }
    }

    return currentModelIndex;
}

int DriveEngine::getCurrentFileItemIndex(FilesManager* manager) const
{
    QList<TreeItemInfo::Data> item = manager->getParser()->getXMLHandler()->getTreeItemInfo()->getFileItems();
    int count = item.count();
    QString fileName(SUi::Inst()->filesView->currentIndex().data().toString());

    int currentFileIndex = 0;

    for(int i = 1; i < count; ++i)
    {
        if(fileName == item[i].name)
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
    foldersManager.reset(new FoldersManager);
    foldersManager->get(GET_FOLDERS);
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
    TreeItemInfo item = *foldersManager->getParser()->getXMLHandler()->getTreeItemInfo();
    int treeItemsIndex = getCurrentFolderItemIndex();

    if (item.getItems().size() > 0) {
        if(item[treeItemsIndex].type == FOLDER_TYPE_STR)
        {
            QString query(item[treeItemsIndex].self);
            query += QString(CONTENTS + MAX_RESULTS);

            filesManager->get(query);
        }
    }
}

bool DriveEngine::folderInFilesView(QString& resourceID)
{
    QList<TreeItemInfo::Data> item = filesManager->getParser()->getXMLHandler()->getTreeItemInfo()->getFileItems();
    int index = getCurrentFileItemIndex(filesManager.data());
    bool isFolder = false;

    QString str(item[index].self);
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

        filesManager->get(query);
    }
}

FoldersManager* DriveEngine::getFoldersManager(void) const
{
    return foldersManager.data();
}

void DriveEngine::slotFilesSortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
{
    qDebug() << "index:" << QString::number(logicalIndex) << " order:" << order;
}

void DriveEngine::slotUpdateFileList()
{
    qDebug("DriveEngine::slotUpdateFileList");
    SDriveEngine::Inst()->showFiles();
}

void DriveEngine::slotDel(QObject* object)
{
    if (object == SUi::Inst()->foldersView)
    {
        if(!elementsStates[EAdditionalViewFocused])
        {
            TreeItemInfo item = *foldersManager->getParser()->getXMLHandler()->getTreeItemInfo();

            if(item[getCurrentFolderItemIndex()].parent != "")
            {
                connect(foldersManager->getOperationsManager(), SIGNAL(signalDelFinished()), this, SLOT(slotDelFinished()));
                foldersManager->del(item[getCurrentFolderItemIndex()].self);
                delItemInTree(item);
            }
        }
    }

    if (object == SUi::Inst()->filesView)
    {
        FilesManager* manager;

        if(elementsStates[EAdditionalViewFocused]) manager = additionalFilesManager.data();
        else manager = filesManager.data();

        QList<TreeItemInfo::Data> itemData = manager->getParser()->getXMLHandler()->getTreeItemInfo()->getFileItems();

        connect(manager->getOperationsManager(), SIGNAL(signalDelFinished()), this, SLOT(slotDelFinished()));
        manager->del(itemData[getCurrentFileItemIndex(manager)].self);
    }
}

void DriveEngine::delItemInTree(TreeItemInfo item)
{
    QTreeWidgetItem *parent = item[getCurrentFolderItemIndex()].pointer->parent();
    int index;

    if (parent)
    {
        index = parent->indexOfChild(SUi::Inst()->foldersView->currentItem());
        delete parent->takeChild(index);
    }
    else
    {
        index = SUi::Inst()->foldersView->indexOfTopLevelItem(SUi::Inst()->foldersView->currentItem());
        delete SUi::Inst()->foldersView->takeTopLevelItem(index);
    }
}

void DriveEngine::slotTriggeredDel()
{
    if(elementsStates[ETrashFocused]) return;
    if(elementsStates[EAdditionalViewFocused] && !elementsStates[EFilesViewFocused]) return;

    QObject* object;

    if(elementsStates[EFolderViewFocused]) object = SUi::Inst()->foldersView;
    else object = SUi::Inst()->filesView;

    slotDel(object);
}

void DriveEngine::slotDelFinished()
{
    if(elementsStates[EAdditionalViewFocused]) slotAdditionalShowFiles(currentAdditionalFolderIndex);
    else showFiles();
}

void DriveEngine::slotCreateFolder()
{
    createFolderDialog = new CreateFolderDialog(parent);

    connect(createFolderDialog, SIGNAL(signalAccept(const QString&)), this, SLOT(slotAcceptCreateFolder(const QString&)));
    connect(createFolderDialog, SIGNAL(signalReject()), this, SLOT(slotRejectCreateFolder()));
    connect(createFolderDialog, SIGNAL(signalFinished(int)), this, SLOT(slotFinishedCreateFolder(int)));

    createFolderDialog->exec();
}

void DriveEngine::slotAcceptCreateFolder(const QString& name)
{
    createFolder(name);  
    delete createFolderDialog;
}

void DriveEngine::slotRejectCreateFolder()
{
    delete createFolderDialog;
}

void DriveEngine::slotFinishedCreateFolder(int result)
{
    delete createFolderDialog;
}

void DriveEngine::createFolder(const QString& name)
{
    TreeItemInfo item = *foldersManager->getParser()->getXMLHandler()->getTreeItemInfo();
    int itemIndex = getCurrentFolderItemIndex();

    if(name == "" || name.contains(QRegExp("[/\\\".<>]")))
    {
        CommonTools::msg("Please enter valid name");
        return;
    }

    foldersManager->createFolder(item[itemIndex].self, name);
    foldersManager->insertFolder(name, item[itemIndex].pointer);
}

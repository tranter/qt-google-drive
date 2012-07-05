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
    filesTransfer.reset(new FilesTransferUI);
    filesUI.reset(new FilesUI);

    setConnections();

    showFolders();
    showAdditionalFolders();

    SUi::inst()->filesView->header()->resizeSection(0, 380);
}

void DriveEngine::setConnections(void)
{
    connect(SUi::inst()->foldersView, SIGNAL(clicked (const QModelIndex&)), this, SLOT(slotFoldersViewClicked(const QModelIndex&)));
    connect(SUi::inst()->filesView, SIGNAL(clicked (const QModelIndex&)), filesUI.data(), SLOT(slotFilesViewClicked(const QModelIndex&)));
    connect(SUi::inst()->additionalFoldersView, SIGNAL(clicked (const QModelIndex&)), filesUI.data(), SLOT(slotAdditionalShowFiles(const QModelIndex&)));
    connect(SUi::inst()->filesView->header(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), this, SLOT(slotFilesSortIndicatorChanged(int, Qt::SortOrder)));
}

void DriveEngine::slotFilesSortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
{
    qDebug() << "index:" << QString::number(logicalIndex) << " order:" << order;
}

OAuth2* DriveEngine::getOAuth2(void) const
{
    return oAuth2.data();
}

int DriveEngine::getCurrentFolderItemIndex(void) const
{
    QTreeWidgetItem* pointer = static_cast<QTreeWidgetItem*>(SUi::inst()->foldersView->currentIndex().internalPointer());
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
    filesUI->showFiles();
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

bool DriveEngine::folderInFilesView(QString& resourceID)
{
    QList<TreeItemInfo::Data> item = filesManager->getParser()->getXMLHandler()->getTreeItemInfo()->getFileItems();
    int index = filesUI->getCurrentFileItemIndex(filesManager.data());
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

FoldersManager* DriveEngine::getFoldersManager(void) const
{
    return foldersManager.data();
}

FilesTransferUI* DriveEngine::getfilesTransferUI(void) const
{
    return filesTransfer.data();
}

void DriveEngine::slotDel(QObject* object)
{
    if (object == SUi::inst()->foldersView)
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

    if (object == SUi::inst()->filesView)
    {
        FilesManager* manager;

        if(elementsStates[EAdditionalViewFocused]) manager = additionalFilesManager.data();
        else manager = filesManager.data();

        QList<TreeItemInfo::Data> itemData = manager->getParser()->getXMLHandler()->getTreeItemInfo()->getFileItems();

        connect(manager->getOperationsManager(), SIGNAL(signalDelFinished()), this, SLOT(slotDelFinished()));
        manager->del(itemData[filesUI->getCurrentFileItemIndex(manager)].self);
    }
}

void DriveEngine::delItemInTree(TreeItemInfo item)
{
    QTreeWidgetItem *parent = item[getCurrentFolderItemIndex()].pointer->parent();
    int index;

    if (parent)
    {
        index = parent->indexOfChild(SUi::inst()->foldersView->currentItem());
        delete parent->takeChild(index);
    }
    else
    {
        index = SUi::inst()->foldersView->indexOfTopLevelItem(SUi::inst()->foldersView->currentItem());
        delete SUi::inst()->foldersView->takeTopLevelItem(index);
    }
}

void DriveEngine::slotTriggeredDel()
{
    if(elementsStates[ETrashFocused]) return;
    if(elementsStates[EAdditionalViewFocused] && !elementsStates[EFilesViewFocused]) return;

    QObject* object;

    if(elementsStates[EFolderViewFocused]) object = SUi::inst()->foldersView;
    else object = SUi::inst()->filesView;

    slotDel(object);
}

void DriveEngine::slotDelFinished()
{
    if(elementsStates[EAdditionalViewFocused]) filesUI->slotAdditionalShowFiles(currentAdditionalFolderIndex);
    else filesUI->showFiles();
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

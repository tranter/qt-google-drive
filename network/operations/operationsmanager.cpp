#include "operationsmanager.h"
#include "core/driveengine.h"
#include "settings/settingsmanager.h"
#include <QStringList>

OperationsManager::OperationsManager(QObject *parent):
    NetworkManager(parent)
{
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

bool OperationsManager::operationPossible(void)
{
    bool is = false;

    int index = SDriveEngine::inst()->getContentMngr()->getPanel()->currentIndex().row();

    if(index >= 0)
    {
        QString itemText = SDriveEngine::inst()->getContentMngr()->getPanel()->currentItem()->text(0);
        if(itemText != PARENT_FOLDER_SIGN) is = true;
    }

    return is;
}

void OperationsManager::slotNewFolder(void)
{
    createFolderDialog = new CreateFolderDialog(SDriveEngine::inst()->getParent());

    connect(createFolderDialog, SIGNAL(signalAccept(const QString&)), this, SLOT(slotAcceptCreateFolder(const QString&)));
    connect(createFolderDialog, SIGNAL(signalReject()), this, SLOT(slotRejectCreateFolder()));
    connect(createFolderDialog, SIGNAL(signalFinished(int)), this, SLOT(slotFinishedCreateFolder(int)));

    createFolderDialog->setNameLabel(tr("New folder name:"));
    createFolderDialog->exec();
}

void OperationsManager::slotRejectCreateFolder(void)
{
    delete createFolderDialog;
}

void OperationsManager::slotFinishedCreateFolder(int result)
{
    Q_UNUSED(result);
    delete createFolderDialog;
}

void OperationsManager::slotCopy(void)
{
    performOperation(&copy);
}

void OperationsManager::slotMove(void)
{    
    performOperation(&move);
}

void OperationsManager::slotDelete(void)
{
    FilePanel *filePanel = SDriveEngine::inst()->getFilePanel(SettingsManager().currentPanel());
    QTreeWidget *treeWidget = filePanel->getFileView();
    QList<int> markedItemIds = filePanel->getMarkedItemIds(treeWidget);
    WebContentManager* webContentManager = dynamic_cast<WebContentManager*> (SDriveEngine::inst()->getContentMngr());
    Items::Data source = webContentManager->getCurrentItem();

    if(markedItemIds.isEmpty())
    {
        if(!operationPossible())
        {
            CommonTools::msg(tr("No files selected"));
            return;
        }

        del.item(source);
    }
    else
    {
        QList<Items::Data> filesData;
        webContentManager->getItemsDataByIndexes(markedItemIds, filesData);

        disconnect(&del, SIGNAL(itemDeleted(Items::Data&)), this, SLOT(slotItemOperationCompleted(Items::Data&)));
        connect(&del, SIGNAL(itemDeleted(Items::Data&)), this, SLOT(slotItemOperationCompleted(Items::Data&)));

        del.items(filesData);
    }
}

void OperationsManager::slotRename(void)
{
    if(!operationPossible())
    {
        CommonTools::msg(tr("No files selected"));
        return;
    }

    QTreeWidgetItem *item = SDriveEngine::inst()->getContentMngr()->getPanel()->currentItem();

    editingItemText = item->text(0);

    item->setFlags(item->flags() | Qt::ItemIsEditable);
    SDriveEngine::inst()->getContentMngr()->getPanel()->editItem(item, 0);

    connect(SDriveEngine::inst()->getContentMngr()->getPanel()->itemDelegate(), SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)), this, SLOT(slotItemEditDone()));
}

void OperationsManager::slotItemEditDone(void)
{
    QTreeWidgetItem *item = SDriveEngine::inst()->getContentMngr()->getPanel()->currentItem();
    WebContentManager* webContentManager = dynamic_cast<WebContentManager*> (SDriveEngine::inst()->getContentMngr());
    Items::Data source = webContentManager->getCurrentItem();

    QString itemTextAfterEditing(item->text(0));

    if(editingItemText != itemTextAfterEditing)
    {
        rename.item(source, itemTextAfterEditing);
        editingItemText.clear();
    }
}

void OperationsManager::slotShare(void)
{
    WebContentManager* webContentManager = dynamic_cast<WebContentManager*> (SDriveEngine::inst()->getContentMngr());
    Items::Data source = webContentManager->getCurrentItem();
    share.file(source);
}

void OperationsManager::slotAcceptCreateFolder(const QString &name)
{
    if(name == "" || name.contains(QRegExp("[/.<>]")) || name.contains(QRegExp("\\\\")) || name.contains(QRegExp("\"")))
    {
        CommonTools::msg(tr("Please enter a valid name"));
        return;
    }

    SettingsManager settingsManager;

    create.folder(name, settingsManager.currentFolderUrl(settingsManager.currentPanel()));

    delete createFolderDialog;
}

void OperationsManager::slotItemOperationCompleted(Items::Data &itemData)
{
    QTreeWidget *treeWidget = SDriveEngine::inst()->getFilePanel(SettingsManager().currentPanel())->getFileView();

    WebContentManager* webContentManager = dynamic_cast<WebContentManager*> (SDriveEngine::inst()->getContentMngr());
    int index = webContentManager->getIndexByItemData(treeWidget, itemData);
    FilePanel *filePanel = SDriveEngine::inst()->getFilePanel(SettingsManager().currentPanel());

    if(index > -1) filePanel->markItem(treeWidget->topLevelItem(index));
}

void OperationsManager::performOperation(Operation *operation)
{
    FilePanel *filePanel = SDriveEngine::inst()->getFilePanel(SettingsManager().currentPanel());
    QTreeWidget *treeWidget = filePanel->getFileView();
    QList<int> markedItemIds = filePanel->getMarkedItemIds(treeWidget);
    SettingsManager settingsManager;
    QString destFolderUrl(settingsManager.currentFolderUrl(settingsManager.oppositePanel()));
    WebContentManager* webContentManager = dynamic_cast<WebContentManager*> (SDriveEngine::inst()->getContentMngr());
    Items::Data source = webContentManager->getCurrentItem();

    if(markedItemIds.isEmpty())
    {
        if(!operationPossible())
        {
            CommonTools::msg(tr("No Files selected"));
            return;
        }

        operation->file(source, destFolderUrl);
    }
    else
    {
        QList<Items::Data> foldersData, filesData;
        webContentManager->getItemsDataByIndexes(markedItemIds, foldersData, filesData);

        if(operation->getOperationId() == ECopy)
        {
            disconnect(&copy, SIGNAL(fileCopied(Items::Data&)), this, SLOT(slotItemOperationCompleted(Items::Data&)));
            connect(&copy, SIGNAL(fileCopied(Items::Data&)), this, SLOT(slotItemOperationCompleted(Items::Data&)));
        }
        if(operation->getOperationId() == EMove)
        {
            disconnect(move.getCopyPart(), SIGNAL(fileCopied(Items::Data&)), this, SLOT(slotItemOperationCompleted(Items::Data&)));
            connect(move.getCopyPart(), SIGNAL(fileCopied(Items::Data&)), this, SLOT(slotItemOperationCompleted(Items::Data&)));
        }

        operation->files(filesData, destFolderUrl);
    }
}






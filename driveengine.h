#ifndef DRIVEENGINE_H
#define DRIVEENGINE_H

#include "oauth2.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"
#include <QFileDialog>
#include <QTreeWidgetItem>
#include "downloadmanager.h"
#include "uploadmanager.h"
#include "filesmanager.h"
#include "foldersmanager.h"
#include "additionalfoldesmanager.h"
#include "createfolderdialog.h"

class FoldersManager;
class FilesManager;
class AdditionalFoldersManager;
class CreateFolderDialog;

class DriveEngine : public QObject
{
    Q_OBJECT
public:
    enum EElementsStates
    {
        EFolderViewFocused = 0,
        EAdditionalViewFocused,
        EFilesViewFocused,
        ETrashFocused,
        EElementsStatesCount
    };

    explicit DriveEngine(QObject *parent = 0);
    ~DriveEngine();

public:
    void init(void);
    OAuth2* getOAuth2(void) const;
    void showFiles(void);
    FoldersManager* getFoldersManager(void) const;

public slots:
    void slotStartLogin();
    void slotStartLoginFromMenu();
    bool slotCheckWorkDir(bool showDlg = true);
    void slotDelFinished();
    void slotCreateFolder();
    void slotAcceptCreateFolder(const QString &name);
    void slotRejectCreateFolder();
    void slotFinishedCreateFolder(int result);
    void slotDel(QObject* object);

private slots:
    void slotDownload(void);
    void slotUpload(void);
    void slotFoldersViewClicked(const QModelIndex& index);
    void slotFilesViewClicked(const QModelIndex&);
    void slotAdditionalShowFiles(const QModelIndex& index);
    void slotFilesSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
    void slotTriggeredDel();
    //void slotFoldersShowed();

private:
    void setConnections(void);
    int getCurrentFolderItemIndex(void) const;
    int getCurrentFileItemIndex(FilesManager* manager) const;
    void download(FilesManager* manager);
    void upload(void);
    bool folderInFilesView(QString &resourceID);
    void showFolders(void);
    void showAdditionalFolders(void);
    void showFilesFromFolderInFilesView(void);
    void delItemInTree(TreeItemInfo item);
    void createFolder(const QString& name);

private:
    QWidget* parent;
    QModelIndex currentAdditionalFolderIndex;

    int currentFolderIndex;
    bool elementsStates[EElementsStatesCount];

    CreateFolderDialog* createFolderDialog;

    QScopedPointer<DownloadFileManager> downloadManager;
    QScopedPointer<UploadFileManager> uploadFileManager;
    QScopedPointer<FoldersManager> foldersManager;
    QScopedPointer<FilesManager> filesManager;
    QScopedPointer<AdditionalFoldersManager> additionalFilesManager;
    QScopedPointer<OAuth2> oAuth2;
};

#endif // DRIVEENGINE_H

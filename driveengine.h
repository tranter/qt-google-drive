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

class FoldersManager;
class FilesManager;
class AdditionalFoldersManager;

class DriveEngine : public QObject
{
    Q_OBJECT
public:

    enum EObjects
    {
        EFilesView = 0 ,
        EFoldersView
    };

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

private slots:
    void slotDownload(void);
    void slotUpload(void);
    void slotFoldersViewClicked(const QModelIndex&);
    void slotFilesViewClicked(const QModelIndex&);
    void slotAdditionalShowFiles(const QModelIndex& index);
    void slotFilesSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
    void slotDel(QObject* object);
    void slotTriggeredDel();
    //void slotFoldersShowed();

private:
    void setConnections(void);
    int getCurrentModelItemIndex(void) const;
    int getCurrentFileItemIndex(FilesManager* manager) const;
    void download(FilesManager* manager);
    void upload(void);
    bool folderInFilesView(QString &resourceID);
    void showFolders(void);
    void showAdditionalFolders(void);
    void showFilesFromFolderInFilesView(void);

private:
    OAuth2* oAuth2;
    QWidget* parent;
    DownloadFileManager* downloadManager;
    UploadFileManager* uploadFileManager;
    FoldersManager* foldersManager;
    FilesManager* filesManager;
    AdditionalFoldersManager* additionalFilesManager;
    bool elementsStates[EElementsStatesCount];
    QModelIndex currentAdditionalFolderIndex;
};

#endif // DRIVEENGINE_H

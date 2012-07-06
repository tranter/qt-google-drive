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
#include "filestransferui.h"
#include "filesui.h"
#include "foldersui.h"
#include "operationsui.h"

class FoldersManager;
class FilesManager;
class AdditionalFoldersManager;
class CreateFolderDialog;
class FilesTransferUI;
class FilesUI;
class FoldersUI;
class OperationsUI;

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

    friend class FilesTransferUI;
    friend class FilesUI;
    friend class FoldersUI;
    friend class OperationsUI;

public:   
    FilesTransferUI* getfilesTransferUI(void) const;
    FoldersManager* getFoldersManager(void) const;
    OAuth2* getOAuth2(void) const;
    OperationsUI* getOperationsUI(void) const;
    void init(void);

public slots:
    void slotStartLogin();
    void slotStartLoginFromMenu();
    bool slotCheckWorkDir(bool showDlg = true);

private:
    void setConnections(void);

private:
    QWidget* parent;
    QModelIndex currentAdditionalFolderIndex;

    int currentFolderIndex;
    bool elementsStates[EElementsStatesCount];

    CreateFolderDialog* createFolderDialog;

    QScopedPointer<FilesTransferUI> filesTransfer;
    QScopedPointer<FilesUI> filesUI;
    QScopedPointer<FoldersUI> foldersUI;
    QScopedPointer<OperationsUI> operationsUI;

    QScopedPointer<DownloadFileManager> downloadManager;
    QScopedPointer<UploadFileManager> uploadFileManager;
    QScopedPointer<FoldersManager> foldersManager;
    QScopedPointer<FilesManager> filesManager;
    QScopedPointer<AdditionalFoldersManager> additionalFilesManager;
    QScopedPointer<OAuth2> oAuth2;
};

#endif // DRIVEENGINE_H

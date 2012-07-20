#ifndef DRIVEENGINE_H
#define DRIVEENGINE_H

#include "oauth2.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTreeWidgetItem>
#include "downloadmanager.h"
#include "uploadmanager.h"
#include "filesmanager.h"
#include "foldersmanager.h"
#include "additionalfoldesmanager.h"
#include "filestransferui.h"
#include "filesui.h"
#include "foldersui.h"
#include "operationsui.h"
#include "checkui.h"
#include "eventhandler.h"

class AdditionalFoldersManager;
class CheckUI;
class CreateFolderDialog;
class FilesManager;
class FilesTransferUI;
class FilesUI;
class FoldersManager;
class FoldersUI;
class OperationsUI;

class DriveEngine : public QObject
{
    Q_OBJECT
public:
    enum EElementsStates
    {
        EFolderViewFocused = 0,
        EAddViewFocused,
        EFilesViewFocused,
        ETrashFocused,
        EElStatesCount
    };

    explicit DriveEngine(QObject *parent = 0);
    ~DriveEngine();

    friend class FilesTransferUI;
    friend class FilesUI;
    friend class FoldersUI;
    friend class OperationsUI;

public:   
    CheckUI* getCheckUI(void) const;
    EventHandler<OperationsUI>* getEventHandler(void) const;
    FilesTransferUI* getfilesTransferUI(void) const;
    FilesUI* getfilesUI(void) const;
    FoldersManager* getFoldersManager(void) const;
    FoldersUI* getFoldersUI(void) const;
    OAuth2* getOAuth2(void) const;
    OperationsUI* getOperationsUI(void) const;
    QWidget* getParent(void) const;
    void init(void);

private:
    void setKeyActions(void);
    void reset();

public slots:
    void slotStartLogin();
    void slotStartLoginFromMenu();

private:
    QWidget* parent;

    bool elStates[EElStatesCount];

    QScopedPointer<AdditionalFoldersManager> addlFilesMngr;
    QScopedPointer<CheckUI> checkUI;
    QScopedPointer<DownloadFileManager> downloadMngr;
    QScopedPointer<EventHandler <OperationsUI> > opEventHandler;
    QScopedPointer<FilesManager> filesMngr;
    QScopedPointer<FilesTransferUI> filesTransferUI;
    QScopedPointer<FilesUI> filesUI;
    QScopedPointer<FoldersManager> foldersMngr;
    QScopedPointer<FoldersUI> foldersUI;
    QScopedPointer<OAuth2> oAuth2;
    QScopedPointer<OperationsUI> opUI;
    QScopedPointer<UploadFileManager> uploadFileMngr;
};

#endif // DRIVEENGINE_H

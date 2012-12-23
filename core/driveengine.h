#ifndef DRIVEENGINE_H
#define DRIVEENGINE_H

#include <QTreeWidgetItem>
#include "network/downloadmanager.h"
#include "network/uploadmanager.h"
#include "gui/forms/mainwindow.h"
#include "ui_mainwindow.h"
#include "network/foldersmanager.h"
#include "gui/additionalfoldesmanager.h"
#include "gui/filestransferui.h"
#include "gui/filesui.h"
#include "gui/foldersui.h"
#include "gui/operationsui.h"
#include "gui/checkui.h"
#include "core/eventhandler.h"
#include "share/enums.h"
#include "gui/forms/filepanel.h"

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
    explicit DriveEngine(QObject *parent = 0);
    ~DriveEngine();

    friend class FilesTransferUI;
    friend class FilesUI;
    friend class FoldersUI;
    friend class OperationsUI;

public:   
    CheckUI* getCheckUI(void) const;
    FilesManager* getFilesMngr(bool opposite = false) const;
    EventHandler<OperationsUI>* getEventHandler(void) const;
    FilesTransferUI* getfilesTransferUI(void) const;
    FilesUI* getfilesUI(void) const;
    FoldersManager* getFoldersMngr(void) const;
    FoldersUI* getFoldersUI(void) const;
    //OAuth2* getOAuth2(void) const;
    OperationsUI* getOpUI(void) const;
    QWidget* getParent(void) const;
    void init(void);
    FilePanel *getFilePanel(EPanels panel) const;
    void updatePanel(const QString &panelName, bool initLoad);

private slots:
    void slotFirstPanelIsLoaded();

private:
    void setKeyActions(void);
    void reset(void);   

//public slots:
//    void slotStartLoginFromMenu(void);

private:
    QWidget* parent;
    QScopedPointer<AdditionalFoldersManager> aFoldersMngr;
    QScopedPointer<CheckUI> checkUI;
    QScopedPointer<DownloadFileManager> downloadMngr;
    QScopedPointer<EventHandler <OperationsUI> > opEventHandler;
    QScopedPointer<FilesManager> filesMngr[EPanelsCount];
    QScopedPointer<FilesTransferUI> filesTransferUI;
    QScopedPointer<FilesUI> filesUI;
    QScopedPointer<FoldersManager> foldersMngr;
    QScopedPointer<FoldersUI> foldersUI;
    //QScopedPointer<OAuth2> oAuth2;
    QScopedPointer<OperationsUI> opUI;
    QScopedPointer<UploadFileManager> uploadFileMngr;
    FilePanel *filesViewLeft;
    FilePanel *filesViewRight;
};

typedef TSingleton<DriveEngine> SDriveEngine;

#endif // DRIVEENGINE_H

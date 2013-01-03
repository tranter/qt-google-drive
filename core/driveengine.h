#ifndef DRIVEENGINE_H
#define DRIVEENGINE_H

#include <QTreeWidgetItem>
#include "network/downloadmanager.h"
#include "network/uploadmanager.h"
#include "network/filesmanager.h"
#include "gui/forms/mainwindow.h"
#include "ui_mainwindow.h"
#include "gui/filestransferui.h"
#include "gui/filesui.h"
#include "gui/checkui.h"
#include "share/enums.h"
#include "gui/forms/filepanel.h"

class AdditionalFoldersManager;
class CheckUI;
class CreateFolderDialog;
class FilesManager;
class FilesTransferUI;
class FilesUI;
class OperationsUI;

class DriveEngine : public QObject
{
    Q_OBJECT
public:
    explicit DriveEngine(QWidget *p = 0);
    ~DriveEngine();

    friend class FilesTransferUI;
    friend class FilesUI;
    friend class FoldersUI;
    friend class OperationsUI;

public:   
    CheckUI* getCheckUI(void) const;
    FilesManager* getFilesMngr(bool opposite = false) const;
    FilesTransferUI* getfilesTransferUI(void) const;
    FilesUI* getfilesUI(void) const;
    QWidget* getParent(void) const;
    void init(void);
    FilePanel *getFilePanel(EPanels panel) const;
    void updatePanel(int panelId, bool initLoad);

private slots:
    void slotFirstPanelIsLoaded(void);
    void slotAccountChanged(int panelNum, const QString& accountName);

private:
    void reset(void);   

private:
    QWidget *parent;
    QScopedPointer<CheckUI> checkUI;
    QScopedPointer<DownloadFileManager> downloadMngr;
    QScopedPointer<FilesManager> filesMngr[EPanelsCount];
    QScopedPointer<FilesTransferUI> filesTransferUI;
    QScopedPointer<FilesUI> filesUI;
    QScopedPointer<UploadFileManager> uploadFileMngr;
    FilePanel *filesViews[EPanelsCount];
};

typedef TSingleton<DriveEngine> SDriveEngine;

#endif // DRIVEENGINE_H

#ifndef DRIVEENGINE_H
#define DRIVEENGINE_H

#include <QTreeWidgetItem>
#include "network/downloadmanager.h"
#include "network/uploadmanager.h"
#include "network/contentmanager.h"
#include "gui/forms/mainwindow.h"
#include "ui_mainwindow.h"
#include "gui/filestransferui.h"
#include "gui/contentui.h"
#include "gui/checkui.h"
#include "share/enums.h"
#include "gui/forms/filepanel.h"

class AdditionalFoldersManager;
class CheckUI;
class CreateFolderDialog;
class ContentManager;
class FilesTransferUI;
class ContentUI;
class OperationsUI;

class DriveEngine : public QObject
{
    Q_OBJECT
public:
    explicit DriveEngine(QWidget *p = 0);
    ~DriveEngine();

    friend class FilesTransferUI;
    friend class ContentUI;
    friend class FoldersUI;
    friend class OperationsUI;

public:   
    CheckUI* getCheckUI(void) const;
    ContentManager* getContentMngr(bool opposite = false) const;
    FilesTransferUI* getfilesTransferUI(void) const;
    ContentUI* getfilesUI(void) const;
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
    QScopedPointer<ContentManager> contentMngr[EPanelsCount];
    QScopedPointer<FilesTransferUI> filesTransferUI;
    QScopedPointer<ContentUI> filesUI;
    QScopedPointer<UploadFileManager> uploadFileMngr;
    FilePanel *filesViews[EPanelsCount];
};

typedef TSingleton<DriveEngine> SDriveEngine;

#endif // DRIVEENGINE_H

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

class FilesTransferUI;
class ContentUI;

class DriveEngine : public QObject
{
    Q_OBJECT
public:
    explicit DriveEngine(QWidget *p = 0);
    ~DriveEngine();

    friend class FilesTransferUI;
    friend class ContentUI;

public:   
    CheckUI *getCheckUI() const;
    ContentManager *getContentMngr(bool opposite = false) const;
    FilesTransferUI *getfilesTransferUI() const;
    ContentUI *getContentUI() const;
    QWidget *getParent() const;
    void init();
    FilePanel *getFilePanel(int panel) const;
    void updatePanel(int panelId, bool initLoad);
    bool isPanelsContentIdentical();

private slots:
    void slotFirstPanelIsLoaded();

private:
    QWidget *parent;
    QScopedPointer<CheckUI> checkUI;
    QScopedPointer<DownloadFileManager> downloadMngr;
    QScopedPointer<ContentManager> contentMngr[EPanelsCount];
    QScopedPointer<FilesTransferUI> filesTransferUI;
    QScopedPointer<ContentUI> contentUI;
    QScopedPointer<UploadFileManager> uploadFileMngr;
    FilePanel *filePanels[EPanelsCount];
};

typedef TSingleton<DriveEngine> SDriveEngine;

#endif // DRIVEENGINE_H

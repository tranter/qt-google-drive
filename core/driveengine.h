#ifndef DRIVEENGINE_H
#define DRIVEENGINE_H

#include <QTreeWidgetItem>
#include "network/downloadmanager.h"
#include "network/uploadmanager.h"
#include "gui/forms/mainwindow.h"
#include "ui_mainwindow.h"
#include "gui/filestransferui.h"
#include "gui/checkui.h"
#include "share/enums.h"
#include "gui/forms/filepanel.h"

class FilesTransferUI;

class DriveEngine : public QObject
{
    Q_OBJECT
public:
    explicit DriveEngine(QWidget *p = 0);
    ~DriveEngine();

    friend class FilesTransferUI;

public slots:
    void slotFirstPanelIsLoaded();
    void slotComputerPanelActivated(int panelNum);

public:   
    CheckUI *getCheckUI() const;
    ContentManager *getContentMngr(bool opposite = false) const;
    FilesTransferUI *getfilesTransferUI() const;
    QWidget *getParent() const;
    void init();
    FilePanel *getFilePanel(int panel) const;
    void updatePanel(int panelId, bool initLoad);
    bool isPanelsContentIdentical();

private:
    QWidget *parent;
    QScopedPointer<CheckUI> checkUI;
    QScopedPointer<DownloadFileManager> downloadMngr;
    QScopedPointer<FilesTransferUI> filesTransferUI;
    QScopedPointer<UploadFileManager> uploadFileMngr;
    FilePanel *filePanels[EPanelsCount];
};

typedef TSingleton<DriveEngine> SDriveEngine;

#endif // DRIVEENGINE_H

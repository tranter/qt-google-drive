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

class FoldersManager;
class FilesManager;

class DriveEngine : public QObject
{
    Q_OBJECT
public:

    explicit DriveEngine(QObject *parent = 0);
    ~DriveEngine();

public:
    void init(void);
    OAuth2* getOAuth2(void) const;
    void showFolders(void);
    void showFiles(void);
    void showFilesFromFolderInFilesView(void);
    FoldersManager* getFoldersManager(void) const;

private:
    void setConnections(void);
    int getCurrentModelItemIndex(void) const;
    int getCurrentFileItemIndex(void) const;

public slots:
    void slotStartLogin();
    void slotStartLoginFromMenu();
    bool slotCheckWorkDir(bool showDlg = true);

private slots:
    void slotDownload(void);
    void slotUpload(void);
    void slotFolderTreeViewClicked(const QModelIndex& index);
    void slotFilesTreeViewClicked(const QModelIndex& index);
    void slotSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);

private:
    OAuth2* oAuth2;
    QWidget* parent;
    DownloadFileManager* downloadManager;
    UploadFileManager* uploadFileManager;
    FoldersManager* foldersManager;
    FilesManager* filesManager;
};

#endif // DRIVEENGINE_H

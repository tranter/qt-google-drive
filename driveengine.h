#ifndef DRIVEENGINE_H
#define DRIVEENGINE_H

#include <QObject>
#include "oauth2.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSslError>
#include <QXmlSimpleReader>
#include "xmlparser.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "treemodel.h"
#include "downloadmanager.h"
#include "settingsdialog.h"
#include "commontools.h"
#include <QFileDialog>
#include "uploadmanager.h"
#include <QTreeWidgetItem>
#include "filesmanager.h"

class FilesManager;

class DriveEngine : public QObject
{
    Q_OBJECT
public:

//    enum EReplies
//    {
//        EFolders = 0,
//        EFiles,
//        ERepliesCount
//    };

    explicit DriveEngine(QObject *parentObj = 0);
    ~DriveEngine();

public:
    void init(void);
    OAuth2* getOAuth2(void) const;
    int getCurrentModelItemIndex(void) const;
    int getCurrentFileItemIndex(void) const;
    void showFiles(void);
    
public slots:
    void slotStartLogin(void);
    bool slotCheckWorkDir(bool showDlg = true);

private slots:
    void slotReplyFinished(QNetworkReply* reply);
    void slotGet(void);

    void slotFoldersReadyRead();
    void slotFoldersError(QNetworkReply::NetworkError error);
    void slotFoldersSslErrors(const QList<QSslError>& errors);

    void slotDownload(void);
    void slotUpload(void);

    void slotTreeViewExpanded(const QModelIndex& index);
    void slotTreeViewCollapsed(const QModelIndex& index);
    void slotTreeViewClicked(const QModelIndex& index);

    void slotResDownloaded(int queryType);

    //void slotItemClicked(QTreeWidgetItem* item, int column);

signals:
    void signalAccessTokenExpired();

private:
    void setConnections(void);
    void settings(void);
    bool parseReply(const QString& str, int type);
    void setModel(void);

private:
    OAuth2* oAuth2;
    QNetworkAccessManager* networkAccessManager;
    QWidget* parent;
    QNetworkReply* reply;
    QNetworkRequest request;
    QString replyStr;
    QString accessToken;
    TreeModel* model;
    XMLParser *parser;
    QXmlSimpleReader* reader;
    DownloadFileManager* downloadManager;
    UploadFileManager* uploadFileManager;
    FilesManager* filesManager;
};

#endif // DRIVEENGINE_H

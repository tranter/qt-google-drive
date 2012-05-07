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

class DriveEngine : public QObject
{
    Q_OBJECT
public:

    enum EReplies
    {
        EFolders = 0,
        EFiles,
        ECount
    };

    explicit DriveEngine(QObject *parentObj = 0);
    ~DriveEngine();

public:
    void init(void);
    OAuth2* getOAuth2(void) const;
    int getCurrentModelItemIndex(void) const;
    
public slots:
    void slotStartLogin(void);

private slots:
    void slotReplyFinished(QNetworkReply* reply);
    void slotGet(void);
    void slotPost(void);

    void slotFoldersReadyRead();
    void slotFoldersError(QNetworkReply::NetworkError error);
    void slotFoldersSslErrors(const QList<QSslError>& errors);

    void slotFilesReadyRead();
    void slotFilesError(QNetworkReply::NetworkError error);
    void slotFilesSslErrors(const QList<QSslError>& errors);

    void slotDownload(void);

private:
    void settings(EReplies eReply);
    void setHeader(QNetworkRequest& request);
    bool parseReply(const QString& str, int type);
    void setModel(void);

private:
    OAuth2* oAuth2;
    QNetworkAccessManager* networkAccessManager;
    QObject* parent;
    QNetworkReply* reply[ECount];
    QNetworkRequest request[ECount];
    QString replyStr[ECount];
    QString accessToken;
    TreeModel* model;
    XMLParser *parser;
    QXmlSimpleReader* reader;
    DownloadFileManager* downloadManager;
};

#endif // DRIVEENGINE_H

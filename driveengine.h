#ifndef DRIVEENGINE_H
#define DRIVEENGINE_H

#include <QObject>
#include "oauth2.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSslError>
#include <QXmlSimpleReader>
#include "xmlparser.h"
#include "xmldomparser.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "treemodel.h"
//#include <QTreeView>

class DriveEngine : public QObject
{
    Q_OBJECT
public:
    explicit DriveEngine(QObject *parentObj = 0);
    ~DriveEngine();

public:
    void init(void);
    
public slots:    
    void slotStartLogin(void);

private slots:
    void slotReplyFinished(QNetworkReply* reply);
    void slotGet(void);
    void slotPost(void);
    void slotReadyRead();
    void slotError(QNetworkReply::NetworkError error);
    void slotSslErrors( const QList<QSslError>& errors);

private:
    void settings(void);
    void setHeader(void);
    bool parseReply(const QString& str);

private:
    OAuth2 oAuth2;
    QNetworkAccessManager* networkAccessManager;
    QObject* parent;
    QNetworkReply *reply;
    QNetworkRequest request;
    QString replyStr;
    QString accessToken;
    TreeModel* model;
    XMLParser *parser;
};

#endif // DRIVEENGINE_H

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

class DriveEngine : public QObject
{
    Q_OBJECT
public:
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
    void slotReadyRead();
    void slotError(QNetworkReply::NetworkError error);
    void slotSslErrors( const QList<QSslError>& errors);
    void slotTest(void);

private:
    void settings(void);
    void setHeader(void);
    bool parseReply(const QString& str);
    void setModel(void);

private:
    OAuth2* oAuth2;
    QNetworkAccessManager* networkAccessManager;
    QObject* parent;
    QNetworkReply *reply;
    QNetworkRequest request;
    QString replyStr;
    QString accessToken;
    TreeModel* model;
    XMLParser *parser;
    QString query;
};

#endif // DRIVEENGINE_H

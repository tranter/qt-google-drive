#ifndef FILESMANAGER_H
#define FILESMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSslError>
#include <QXmlSimpleReader>
#include "xmlparser.h"
#include "mainwindow.h"
//#include "treemodel.h"

class FilesManager : public QObject
{
    Q_OBJECT
public:
    explicit FilesManager(QObject *parent = 0);

public:
    void getFiles(const QString& query);
    XMLParser* getParser(void) const;

private slots:
    void slotReplyFinished(QNetworkReply* reply);
    void slotReadyRead();
    void slotError(QNetworkReply::NetworkError error);
    void slotSslErrors( const QList<QSslError>& errors);
    void slotResDownloaded(int queryType);

private:
    bool parseReply(const QString& str);
    void show(void);

private:
    QNetworkAccessManager* networkAccessManager;
    QNetworkReply *reply;
    QNetworkRequest request;
    QString replyStr;
    XMLParser* parser;
    QList<QTreeWidgetItem*> items;
};

#endif // FILESMANAGER_H

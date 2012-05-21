#ifndef FILESMANAGER_H
#define FILESMANAGER_H

#include <QXmlSimpleReader>
#include "xmlparser.h"
#include "mainwindow.h"
#include "networkmanager.h"

class FilesManager :  public NetworkManager
{
    Q_OBJECT
public:
    explicit FilesManager(QObject *parent = 0);

public:
    void getFiles(const QString& url);
    XMLParser* getParser(void) const;

public slots:
    virtual void slotReplyFinished(QNetworkReply* reply);

private slots:
    void slotResDownloaded(int queryType);

private:
    bool parseReply(const QString& str);
    void show(void);

private:
    XMLParser* parser;
    QList<QTreeWidgetItem*> items;
    bool firstRequest;
};

#endif // FILESMANAGER_H

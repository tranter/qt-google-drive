#ifndef FOLDERSMANAGER_H
#define FOLDERSMANAGER_H

#include <QXmlSimpleReader>
#include "xmlparser.h"
#include "mainwindow.h"
#include "networkmanager.h"

class FoldersManager : public NetworkManager
{
    Q_OBJECT
public:
    explicit FoldersManager(QObject *parent = 0);
    
public:
    void getFolders(const QString& url);
    XMLParser* getParser(void) const;

public slots:
    virtual void slotReplyFinished(QNetworkReply* reply);
    virtual void slotError(QNetworkReply::NetworkError error);
    void slotResDownloaded(int queryType);

signals:
    void signalAccessTokenRequired();

private:
    void buildTree(const QString& searchStr, QTreeWidgetItem* parent);
    void fillTree(QList<QString> names, QList<QString> iconPathes, QTreeWidgetItem* parent, QList<int> indexes);
    bool parseReply(const QString& str);
    void show(void);

private:
    XMLParser* parser;
    QList<QTreeWidgetItem*> items;
    bool firstRequest;
    QList<TreeItemInfo::Data> folderItems;
};

#endif // FOLDERSMANAGER_H

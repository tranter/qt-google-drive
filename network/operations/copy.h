#ifndef COPY_H
#define COPY_H

#include "network/networkmanager.h"
#include "operation.h"

class Copy : public NetworkManager, public Operation
{
    Q_OBJECT
public:
    explicit Copy(QObject *parent = 0);

signals:
    void fileCopied(Items::Data &sourceData, bool isLastFile = false);

protected slots:
    void slotPostFinished(QNetworkReply *reply);

public:
    void file(const Items::Data &sourceData, const QString &destFolderUrlData);
    void files(const QList<Items::Data> &sourcesData, const QString &destFolderUrlData);

private:
    QString destFolderUrlData;
};

#endif // COPY_H

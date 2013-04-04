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
    void file(const Items::Data &source, const QString &destination);
    void files(const QList<Items::Data> &sources, const QString &destination);

private:
    QString destFolder;
};

#endif // COPY_H

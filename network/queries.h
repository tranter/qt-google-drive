#ifndef QUERIES_H
#define QUERIES_H

#include <QObject>
#include <QUrl>

class Queries : public QObject
{
     Q_OBJECT
public:
    Queries();

public:
    void userAboutInfo(QString &userInfoQuery, QString &aboutInfoQuery);
    QByteArray copyWebFileData(const QString &sourceName, const QString &destFolderUrl);
    QUrl constructCopyWebFileUrl(const QString &url);
};


#endif // QUERIES_H

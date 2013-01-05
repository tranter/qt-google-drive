#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QNetworkAccessManager>

class Download : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit Download(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // DOWNLOAD_H

#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QNetworkAccessMnager>

class Download : public QNetworkAccessMnager
{
    Q_OBJECT
public:
    explicit Download(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // DOWNLOAD_H

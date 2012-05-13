#include "networkmanager.h"
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent) :
    QObject(parent),
    networkManager(new QNetworkAccessManager),
    state(EReady)
{
    //progressDialog.setParent(static_cast<QWidget*>(parent));
    progressDialog.setCancelButton(0);
}

NetworkManager::~NetworkManager()
{
    if(networkManager) delete networkManager;
}

void NetworkManager::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug() << "bytesReceived =" << bytesReceived << "bytesTotal =" << bytesTotal;

    progressDialog.setMaximum(bytesTotal);
    progressDialog.setValue(bytesReceived);
}

void NetworkManager::downloadFinished()
{
    progressDialog.hide();
    state = EReady;
    file.flush();
    file.close();
}

void NetworkManager::downloadReadyRead()
{
    file.write(reply->readAll());
}

void NetworkManager::uploadProgress( qint64 bytesSent, qint64 bytesTotal )
{
    qDebug() << "bytesSent =" << bytesSent << "bytesTotal =" << bytesTotal;
    progressDialog.setMaximum(bytesTotal);
    progressDialog.setValue(bytesSent);
}

void NetworkManager::uploadFinished()
{
    qDebug() << "uploadFinished";
    progressDialog.hide();
    state = EReady;
}

void NetworkManager::slotError(QNetworkReply::NetworkError error)
{
    qDebug() << "slotError error = " << error;
}

void NetworkManager::slotSslErrors(const QList<QSslError>& errors)
{
    qDebug() << "slotSslErrors error";

    foreach(const QSslError& e,errors)
    {
        qDebug() << "error = " << e.error();
    }
}

void NetworkManager::startDownload(QUrl url, QString& fileName, const QString& fileType)
{
    type = fileType;
    file.setFileName(fileName);

    setStartSettings(url);
    setDownloadSettings();

    file.open(QIODevice::WriteOnly);

    reply = networkManager->get(request);

    connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(downloadReadyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
}

void NetworkManager::startUpload(QUrl url, const QString& fileName)
{
    file.setFileName(fileName);

    setStartSettings(url);
    setUploadSettings();

    reply = networkManager->post(request, uploadContent);
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(postFinished(QNetworkReply*)));
}

NetworkManager::EStates NetworkManager::getState(void) const
{
    return state;
}

void NetworkManager::setState(NetworkManager::EStates currentState)
{
    state = currentState;
}

void NetworkManager::setStartSettings(QUrl url)
{
    state = EBusy;
    progressDialog.show();
    request.setUrl(url);
}

void NetworkManager::postFinished(QNetworkReply* reply)
{
    qDebug() << "postFinished";

    if (reply->error())
    {
        qDebug() << "postFinished. Reply with the error";
        return;
    }

    setPostFinishedSettings(reply);
}

void NetworkManager::doPutRequest(const QString & url,const QByteArray& data)
{
    request.setUrl(QUrl(url));
    reply = networkManager->put(request,data);

    connect(reply, SIGNAL(finished()), this, SLOT(uploadFinished()));
    connect(reply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(uploadProgress(qint64,qint64)));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(sslErrors(const QList<QSslError>&)),this, SLOT(slotSslErrors(const QList<QSslError>&)));
}

void NetworkManager::setDownloadSettings(void) {}
void NetworkManager::setUploadSettings(void) {}
void NetworkManager::setPostFinishedSettings(QNetworkReply* reply) {}

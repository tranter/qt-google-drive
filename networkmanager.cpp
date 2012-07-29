#include "networkmanager.h"
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent) :
    QObject(parent),
    state(EReady),
    operationCanceled(false)
{
    this->parent = parent;
}

void NetworkManager::init(void)
{
    networkManager.reset(new QNetworkAccessManager(this));
}

QNetworkAccessManager* NetworkManager::getNetworkManager(void) const
{
    return  networkManager.data();
}

NetworkManager::~NetworkManager()
{
    qDebug() << "NetworkManager::~NetworkManager()";
}

void NetworkManager::slotDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    progressBarDialog.setMaximum(bytesTotal);
    progressBarDialog.setValue(bytesReceived);
}

void NetworkManager::slotDownloadFinished()
{
    progressBarDialog.hide();
    state = EReady;
    file.flush();
    file.close();
}

void NetworkManager::slotDownloadReadyRead()
{
    file.write(reply->readAll());
}

void NetworkManager::slotReplyReadyRead()
{
    replyStr.append(reply->readAll());
}

void NetworkManager::slotUploadProgress( qint64 bytesSent, qint64 bytesTotal )
{
    progressBarDialog.setMaximum(bytesTotal);
    progressBarDialog.setValue(bytesSent);
}

void NetworkManager::slotUploadFinished()
{
    progressBarDialog.hide();
    state = EReady;
}

void NetworkManager::slotError(QNetworkReply::NetworkError error)
{
    qDebug() << "NetworkManager::slotError error code:" << error;

    if(error == QNetworkReply::AuthenticationRequiredError)
    {
        reply->abort();
        emit signalAccessTokenRequired();
    }

    if(error == QNetworkReply::UnknownNetworkError)
        qDebug() << "\n*******************\nIf this error occur, please make sure that you have openssl installed (also you can try just copy libeay32.dll and ssleay32.dll files from Qt SDK QtCreator/bin folder into your folder where your program .exe file located (tested on non-static compilation only))\n*******************\n";
}

void NetworkManager::slotSslErrors(const QList<QSslError>& errors)
{
    foreach(const QSslError& e,errors)
    {
        qDebug() << "error:" << e.error();
    }
}

void NetworkManager::startDownload(QUrl url, QString& fileName, const QString& type)
{
    init();
    fileType = type;

    setStartSettings(url, fileName, "Downloading file: ");
    setDownloadSettings();

    file.open(QIODevice::WriteOnly);

    reply = networkManager->get(request);

    connect(reply, SIGNAL(finished()), this, SLOT(slotDownloadFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(slotDownloadReadyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(slotDownloadProgress(qint64,qint64)));
    connectErrorHandlers();
}

void NetworkManager::startUpload(QUrl url, const QString& fileName)
{    
    init();
    setStartSettings(url, fileName, "Uploading file: ");
    setUploadSettings();

    reply = networkManager->post(request, uploadContent);
    connect(networkManager.data(), SIGNAL(finished(QNetworkReply*)), this, SLOT(slotPostFinished(QNetworkReply*)));
    connectErrorHandlers();
}

NetworkManager::EStates NetworkManager::getState(void) const
{
    return state;
}

void NetworkManager::setState(NetworkManager::EStates currentState)
{
    state = currentState;
}

void NetworkManager::connectErrorHandlers(void)
{
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(sslErrors(const QList<QSslError>&)),this, SLOT(slotSslErrors(const QList<QSslError>&)));
}

void NetworkManager::setStartSettings(QUrl url, const QString& fileName, const QString& progressBarDialogInfoText)
{
    state = EBusy;

    operationCanceled = false;
    file.setFileName(fileName);
    QFileInfo fi(file.fileName());

    progressBarDialog.setParent(static_cast<QWidget*>(parent), Qt::Dialog);
    progressBarDialog.setText(progressBarDialogInfoText + fi.fileName());
    progressBarDialog.show();

    connect(&progressBarDialog, SIGNAL(signalProgressCanceled()), this, SLOT(slotProgressCanceled()));

    request.setUrl(url);
}

void NetworkManager::slotProgressCanceled()
{
    operationCanceled = true;
    reply->abort();
}

void NetworkManager::slotPostFinished(QNetworkReply* reply)
{
    qDebug() << "slotPostFinished";

    if (reply->error())
    {
        qDebug() << "slotPostFinished. Reply with the error";
        return;
    }

    setPostFinishedSettings(reply);
}

void NetworkManager::putRequest(const QString & url,const QByteArray& data)
{
    request.setUrl(QUrl(url));

    reply = networkManager->put(request,data);

    connect(reply, SIGNAL(finished()), this, SLOT(slotUploadFinished()));
    connect(reply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(slotUploadProgress(qint64,qint64)));
    connectErrorHandlers();
}

void NetworkManager::sendRequest(const QString & url)
{
    qDebug() << "NetworkManager::sendRequest url:" << url;

    init();
    request.setUrl(QUrl(url));

    reply = networkManager->get(request);

    connect(networkManager.data(), SIGNAL(finished(QNetworkReply*)),this, SLOT(slotReplyFinished(QNetworkReply*)));
    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReplyReadyRead()));
    connectErrorHandlers();
}

void NetworkManager::delRes(const QString& url)
{
    qDebug() << " NetworkManager::delRes url:" << url;

    init();
    request.setUrl(QUrl(url));

    reply = networkManager->deleteResource(request);

    connect(networkManager.data(), SIGNAL(finished(QNetworkReply*)),this, SLOT(slotReplyFinished(QNetworkReply*)));
    connectErrorHandlers();
}

const NetworkManager* NetworkManager::self(void) const
{
    return this;
}

QNetworkRequest NetworkManager::getRequest(void) const
{
    return request;
}

void NetworkManager::setDownloadSettings(void) {}
void NetworkManager::setUploadSettings(void) {}
void NetworkManager::setPostFinishedSettings(QNetworkReply*) {}
void NetworkManager::slotReplyFinished(QNetworkReply*){}

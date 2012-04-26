#include "driveengine.h"
#include <QDebug>

DriveEngine::DriveEngine(QObject *parentObj) :
    QObject(parentObj),
    networkAccessManager(NULL),
    parent(parentObj)
{
}

DriveEngine::~DriveEngine()
{
    if(networkAccessManager)
        delete networkAccessManager;
}

void DriveEngine::slotStartLogin(void)
{
    qDebug() << "startLogin";
    oAuth2.startLogin(true);
}

void DriveEngine::init(void)
{
    request.setUrl(QUrl("https://www.googleapis.com/drive/v1/files/{0}"));
    request.setRawHeader("User-Agent", "Google Drive client");

    networkAccessManager = new QNetworkAccessManager(this);

    connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotReplyFinished(QNetworkReply*)));
    connect(parent, SIGNAL(testClick()), this,  SLOT(slotGetFile()));
}

void DriveEngine::slotReplyFinished(QNetworkReply* reply)
{
    qDebug() << "replyFinished";
    qDebug() << "--------------> replyStr" << replyStr;
}

void DriveEngine::slotGetFile(void)
{
    qDebug() << "slotGetFile";
    reply = networkAccessManager->get(request);

    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(sslErrors(const QList<QSslError>&)),this, SLOT(slotSslErrors( const QList<QSslError>&)));
}

void DriveEngine::slotReadyRead()
{
    qDebug() << "slotReadyRead";
    replyStr.append(reply->readAll());
}

void DriveEngine::slotError(QNetworkReply::NetworkError error)
{
    qDebug() << "slotError error = " << error;
}

void DriveEngine::slotSslErrors(const QList<QSslError>& errors)
{
    qDebug() << "slotError error";

    foreach(const QSslError& e,errors)
    {
     qDebug() << "error = " << e.error();
    }
}

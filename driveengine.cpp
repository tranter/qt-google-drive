#include "driveengine.h"
#include <QDebug>
#include <QSettings>
#include "AppRegData.h"

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
    networkAccessManager = new QNetworkAccessManager(this);

    connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotReplyFinished(QNetworkReply*)));
    connect(parent, SIGNAL(testClick()), this, SLOT(slotGetFile()));
}

void DriveEngine::slotReplyFinished(QNetworkReply* reply)
{
    qDebug() << "replyFinished";
    qDebug() << "--------------> replyStr" << replyStr;
}

void DriveEngine::slotGetFile(void)
{
    qDebug() << "slotGetFile";

    QSettings settings(COMPANY_NAME, APP_NAME);
    accessToken = settings.value("access_token").toString();

    qDebug() << "accessToken = " << accessToken;

    request.setRawHeader("User-Agent", "Google Drive client");
    request.setRawHeader("Content-Type", "application/json");
    //request.setRawHeader("Authorization","Bearer");

    //QString query = QString("https://www.googleapis.com/drive/v1/files?access_token=%1").arg(accessToken);
    //QString query = QString("https://www.googleapis.com/oauth2/v1/drive?access_token=%1").arg(accessToken);
    //QString query = QString("https://www.googleapis.com/oauth2/v1/tokeninfo?access_token=%1").arg(accessToken);
    QString query = QString("https://www.googleapis.com/oauth2/v1/userinfo?access_token=%1").arg(accessToken);

    qDebug() << "query = " << query;

    request.setUrl(QUrl(query));

//    QByteArray data;
//    data.append("access_token=" + accessToken);
//    data.append("&title=pets");
//    //data.append("&parentsCollection=[{\"id\":\"0ADK06pfg\"}]");
//    data.append("&mimeType=application/vnd.google-apps.folder");
//    reply = networkAccessManager->post(request, data);

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

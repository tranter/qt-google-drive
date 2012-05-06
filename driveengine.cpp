#include "driveengine.h"
#include <QDebug>
#include <QSettings>
#include "AppRegData.h"

DriveEngine::DriveEngine(QObject *parentObj) :
    QObject(parentObj),
    networkAccessManager(NULL),
    parent(parentObj),
    model(NULL),
    parser(NULL),
    oAuth2(NULL)
{
}

DriveEngine::~DriveEngine()
{
    if(networkAccessManager) delete networkAccessManager;
    if(model) delete model;
    if(parser) delete parser;
    if(oAuth2) delete oAuth2;
}

void DriveEngine::slotStartLogin(void)
{
    oAuth2->startLogin(true);
}

void DriveEngine::init(void)
{
    if(networkAccessManager)
    {
        delete networkAccessManager;
        networkAccessManager = NULL;
    }

    networkAccessManager = new QNetworkAccessManager(this);

    query = GET_FOLDERS;

    if(oAuth2)
    {
        delete oAuth2;
        oAuth2 = NULL;
    }

    oAuth2 = new OAuth2;

    connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotReplyFinished(QNetworkReply*)));
    connect(parent, SIGNAL(siganalGet()), this, SLOT(slotGet()));
    connect(parent, SIGNAL(signalPost()), this, SLOT(slotPost()));
}

void DriveEngine::slotReplyFinished(QNetworkReply* reply)
{
    qDebug() << "--------------> replyStr" << replyStr;

    if(parseReply(replyStr)) setModel();
    else qDebug() << "parse NOT OK";
}

void DriveEngine::setModel(void)
{
    if(model)
    {
        delete model;
        model = NULL;
    }

    QList<QVariant> rootData;

    /*
      add other columns into rootData here, if necessary (i.e. columns with titles which contain additional info about items)
      (example: rootData << TREE_VIEW_MAIN_TITLE << OTHER_COLIMN_TITLE1 <<  OTHER_COLIMN_TITLE2;)
    */

    rootData << TREE_VIEW_MAIN_TITLE;

    model = new TreeModel(rootData, parser->getTreeItemInfo());
    UiInstance::ui->discTreeView->setModel(model);
}

void DriveEngine::slotGet(void)
{
    setHeader();

    request.setUrl(QUrl(query));
    reply = networkAccessManager->get(request);

    settings();
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

void DriveEngine::slotPost(void)
{
    qDebug() << "slotPost";

    //    setHeader();

    //    QString postQuery = "https://www.googleapis.com/drive/v1/files";

    //    request.setUrl(QUrl(postQuery));

    //    QByteArray data;
    //    data.append("&title=pets");
    //    data.append("&mimeType=application/vnd.google-apps.folder");
    //    QUrl dataPart;
    //    dataPart.setEncodedUrl(data);

    //    reply = networkAccessManager->post(request, data);
    //    qDebug() << "URL = " << postQuery + QString(data);

    //    settings();
}

void DriveEngine::settings(void)
{
    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(sslErrors(const QList<QSslError>&)),this, SLOT(slotSslErrors( const QList<QSslError>&)));
}

void DriveEngine::setHeader(void)
{
    QSettings settings(COMPANY_NAME, APP_NAME);
    accessToken = settings.value("access_token").toString();

    request.setRawHeader("User-Agent", "Google Drive client");
    request.setRawHeader("GData-Version", "3.0");
    request.setRawHeader("Authorization",(QString("OAuth %1").arg(accessToken)).toLatin1());
}

bool DriveEngine::parseReply(const QString& str)
{
    QXmlSimpleReader reader;
    QXmlInputSource source;

    if(parser)
    {
        delete parser;
        parser = NULL;
    }

    parser = new XMLParser(FOLDER_TYPE);

    source.setData(str.toAscii());

    reader.setContentHandler(parser);
    reader.setErrorHandler(parser);

    return reader.parse(&source);
}

OAuth2* DriveEngine::getOAuth2(void) const
{
    return oAuth2;
}

void DriveEngine::slotTest(void)
{
    //qDebug() << "getCurrentModelItemIndex = " << QString::number(getCurrentModelItemIndex());
}

int DriveEngine::getCurrentModelItemIndex(void) const
{
    TreeItem *item = static_cast<TreeItem*>(UiInstance::ui->discTreeView->currentIndex().internalPointer());
    int currentModelIndex = 0;

    for(int i = 1; i < parser->getTreeItemInfo()->items.count(); ++i)
    {
        if(parser->getTreeItemInfo()->items[i].item == item)
        {
          currentModelIndex = i;
          break;
        }
    }

    return currentModelIndex;
}

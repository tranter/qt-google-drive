#include "uploadmanager.h"
#include <QDebug>

UploadFileManager::UploadFileManager(QObject *parent) :
    QObject(parent),
    networkManager(new QNetworkAccessManager),
    state(EReady),
    allowPut(true)
{
    //progressDialog.setParent(static_cast<QWidget*>(parent));
    progressDialog.setCancelButton(0);

}

UploadFileManager::~UploadFileManager()
{
    if(networkManager) delete networkManager;
}

void UploadFileManager::uploadProgress( qint64 bytesSent, qint64 bytesTotal )
{
    qDebug() << "bytesSent =" << bytesSent << "bytesTotal =" << bytesTotal;
    progressDialog.setMaximum(bytesTotal);
    progressDialog.setValue(bytesSent);
}

void UploadFileManager::uploadFinished()
{
    qDebug() << "-------------> uploadFinished";
    progressDialog.hide();
    state = EReady;
    emit signalUpdateModel();
}

void UploadFileManager::startUpload(const QString& fileName, QUrl uploadUrl, const QString& accessToken)
{
    state = EBusy;
    progressDialog.show();
    access_token = accessToken;
    file.setFileName(fileName);

    QFileInfo fi(fileName);
    QString ext = fi.suffix();
    QString title = fi.fileName();
    QString contentType = getContentTypeByExtension(ext);

    QString metadata = QString(
                "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                "<entry xmlns=\"http://www.w3.org/2005/Atom\" xmlns:docs=\"http://schemas.google.com/docs/2007\">"
                "<title>%1</title>"
                "</entry>"
                ).arg(title);

    QByteArray content = metadata.toLatin1();

    request.setRawHeader("GData-Version", "3.0");
    request.setRawHeader("Authorization", (QString("OAuth %1").arg(accessToken)).toLatin1());
    request.setRawHeader("Content-Type", "application/atom+xml");
    request.setRawHeader("Content-Length", QString::number(content.size()).toLatin1());

    request.setRawHeader("X-Upload-Content-Length", (QString("%1").arg(fi.size())).toLatin1());
    request.setRawHeader("X-Upload-Content-Type", contentType.toLatin1());

    QString url(uploadUrl.toString());
    url += "/?convert=false";

    request.setUrl(QUrl(url));
    reply = networkManager->post(request, content);

    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(postFinished(QNetworkReply*)));
}

void UploadFileManager::postFinished(QNetworkReply* reply)
{
    qDebug() << "postFinished";

    if (reply->error())
    {
        qDebug() << "Reply with the error!!!!!";
        return;
    }

    QString url = reply->url().toString();
    QString location = reply->rawHeader("Location");
    QString filePath = file.fileName();

    if (!location.isEmpty())
    {
        if(allowPut)
        {
            allowPut = false;
            QFileInfo fi(filePath);
            QString ext = fi.suffix().toLower();
            QString contentType = getContentTypeByExtension(ext);

            file.open(QIODevice::ReadOnly);
            QByteArray arr = file.readAll();
            file.close();

            qlonglong fileSize = arr.count();

            request.setRawHeader("Content-Type", contentType.toLatin1());
            request.setRawHeader("Content-Length", (QString("%1").arg(fileSize)).toLatin1());
            request.setRawHeader("Content-Range", (QString("bytes 0-%1/%2").arg(fileSize-1).arg(fileSize)).toLatin1());

            doPutRequest(location,arr);
        }
    }
}

void UploadFileManager::doPutRequest(const QString & url,const QByteArray& data)
{
    request.setUrl(QUrl(url));
    reply = networkManager->put(request,data);

    connect(reply, SIGNAL(finished()), this, SLOT(uploadFinished()));
    connect(reply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(uploadProgress(qint64,qint64)));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(sslErrors(const QList<QSslError>&)),this, SLOT(slotSslErrors(const QList<QSslError>&)));
}


void UploadFileManager::slotError(QNetworkReply::NetworkError error)
{
    qDebug() << "slotError error = " << error;
}

void UploadFileManager::slotSslErrors(const QList<QSslError>& errors)
{
    qDebug() << "slotSslErrors error";

    foreach(const QSslError& e,errors)
    {
        qDebug() << "error = " << e.error();
    }
}

UploadFileManager::EStates UploadFileManager::getState(void) const
{
    return state;
}

void UploadFileManager::setState(UploadFileManager::EStates currentState)
{
    state = currentState;
}

QString UploadFileManager::getContentTypeByExtension(const QString& ext)
{
    QString contentType;

    if(ext == "doc" || ext == "docx") contentType = "application/msword";
    if(ext == "xls") contentType = "application/vnd.ms-excel";
    if(ext == "ppt" || ext == "pptx") contentType = "application/vnd.ms-powerpoint";
    if(ext == "pdf") contentType = "application/pdf";
    if(ext == "exe") contentType = "application/x-msdos-program";
    if(ext == "rar") contentType = "application/rar";
    if(ext == "png") contentType = "image/png";
    if(ext == "png") contentType = "application/rtf";
    if(ext == "tar") contentType = "application/x-tar";
    if(ext == "zip") contentType = "application/zip";
    if(ext == "") contentType = "";
    if(ext == "jpeg" || ext == "jpg" || ext == "jpe") contentType = "image/jpeg";
    if(ext == "gif") contentType = "image/gif";
    if(ext == "wav") contentType = "application/x-wav";
    if(ext == "tiff" || ext == "tif") contentType = "image/tiff";
    if(ext == "txt" || ext == "cpp" || ext == "h" || ext == "c") contentType = "text/plain";
    if(ext == "mpeg" || ext == "mpg" || ext == "mpe" ) contentType = "video/mpeg";
    if(ext == "qt" || ext == "mov") contentType = "video/quicktime";
    if(ext == "qvi") contentType = "video/x-msvideo";
    if(ext == "video/x-sgi-movie") contentType = "movie";
    if(ext == "exe") contentType = "application/x-msdos-program";

    return contentType;
}

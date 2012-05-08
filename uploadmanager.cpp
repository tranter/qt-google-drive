#include "uploadmanager.h"
#include <QDebug>

UploadFileManager::UploadFileManager(QObject *parent) :
    QObject(parent),
    networkManager(new QNetworkAccessManager),
    state(EReady)
{
    progressDialog.setParent(static_cast<QWidget*>(parent));
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
    progressDialog.hide();
    state = EReady;
//    file.flush();
//    file.close();
}

void UploadFileManager::uploadReadyRead()
{
    //file.write(reply->readAll());
}

void UploadFileManager::startUpload(const QString& fileName)
{
    state = EBusy;

    qDebug() << "UploadFileManager::startUpload fileName" << fileName;

//    file.setFileName(fileName);
//    file.open(QIODevice::WriteOnly);

//    qDebug() << "fileName = " << fileName;

//    progressDialog.show();
//    CommonTools::setHeader(request);
//    request.setUrl(url);

//    qDebug() << "url = " << request.url();

//    reply = networkManager->get(request);

//    connect(reply, SIGNAL(finished()), this, SLOT(uploadFinished()));
//    connect(reply, SIGNAL(readyRead()), this, SLOT(uploadReadyRead()));
//    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
}

UploadFileManager::EStates UploadFileManager::getState(void) const
{
    return state;
}

void UploadFileManager::setState(UploadFileManager::EStates currentState)
{
    state = currentState;
}






/*

void Form::
uploadExternFile()
{
    if( ! checkActiveAccounts() ) return;

    if( m_lastSelectedAccount < 0 ) return;
    Pair & p = m_docsDataManagers[m_lastSelectedAccount];
    qDebug() << Q_FUNC_INFO << m_lastSelectedAccount;

    QSettings settings("ICS", "Google API Docs Client");
    QString lastDir = settings.value("google_last_dir", "").toString();

    QString filePath = QFileDialog::getOpenFileName(
        this, trUtf8("Uploading file"),
        lastDir.isEmpty() ? QDir::homePath() : lastDir,
        trUtf8("Documents(*.%1);;Presentations(*.%2);;Spreadsheets(*.%3);;Portable Document Format(*.%4);;All files(*)")
            .arg(FileTools::defaultDocumentFormat,      FileTools::defaultPresentationFormat,
                 FileTools::defaultSpreadsheetFormat,   FileTools::defaultPortableFormat)
    );

    if( filePath.isEmpty() )
        return;

    QFileInfo finfo(filePath);

    QString ext = finfo.suffix().toLower();
    if( ! FileTools::supportedExtension( ext ) )
    {
        QMessageBox::warning(this, trUtf8("Error"), trUtf8("Unsupported file format: %1").arg(ext));
        return;
    }

    settings.setValue("google_last_dir", finfo.absolutePath());

    p.docsDataManager->uploadNewFile(filePath, p.oauth2->accessToken());
}




void DocsDataManager::uploadNewFile(const QString & filePath, const QString& access_token)
{
    qDebug() << Q_FUNC_INFO << m_strResumableCreateMedia;

    QFileInfo fi(filePath);
    QString ext = fi.suffix();
    QString title = fi.completeBaseName();

    if( ! FileTools::supportedExtension( ext ) )
    {
        QMessageBox::warning(0, tr("Warning"), tr("Unsupported file format: %1").arg(ext));
        return;
    }

    QString contentType = FileTools::getContentTypeByExtension(ext);

    QString metadata = QString(
                "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                "<entry xmlns=\"http://www.w3.org/2005/Atom\" xmlns:docs=\"http://schemas.google.com/docs/2007\">"
                "<title>%1</title>"
                "</entry>"
                ).arg(title);//, docType);
    QByteArray content = metadata.toLatin1();

    m_filesAwaitingToAddInfo.insert(filePath);

    QNetworkRequest request;
    request.setUrl(QUrl(m_strResumableCreateMedia));
    request.setRawHeader("GData-Version", "3.0");
    request.setRawHeader("Authorization", (QString("OAuth %1").arg(access_token)).toLatin1());
    request.setRawHeader("Content-Type", "application/atom+xml");
    request.setRawHeader("Content-Length", QString::number(content.size()).toLatin1());

    request.setRawHeader("X-Upload-Content-Length", (QString("%1").arg(fi.size())).toLatin1());
    request.setRawHeader("X-Upload-Content-Type", contentType.toLatin1());

    m_filesToUpload.insert(m_strResumableCreateMedia, filePath);

    PtrRI p(new RequestInfo());
    p->set_branchPos(0);
    p->set_action(A_UPLOAD_FILE);
    m_idRequests.append(p);

    WrapperRI* pRI = new WrapperRI();   //pointer need to be deleted in reply handler because no parent QObject* present
    pRI->m_pRI = p;
    request.setOriginatingObject(pRI);


    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_pNetworkAccessManager->post(request, content);
}



  */


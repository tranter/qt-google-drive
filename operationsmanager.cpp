#include "operationsmanager.h"
#include "commontools.h"
#include <QDebug>
#include <QStringList>

OperationsManager::OperationsManager(QObject *parent):
     NetworkManager(parent)
{
}

void OperationsManager::del(const QString& resUrl)
{
    CommonTools::setHeader(request);
    request.setRawHeader("If-Match", "*");
    delRes(getFileQuery(resUrl));
}

void OperationsManager::createFolder(const QString& folderUrl, const QString& name)
{
    QString url(folderUrl + "/contents");

//    CommonTools::setHeader(request);
//    request.setRawHeader("If-Match", "*");
//    delRes(getFileQuery(url));



//    QUrl url;
//    QNetworkRequest request;

//    if (collection_id.contains("#my-collection")) {
//        url = "https://docs.google.com/feeds/default/private/full";
//    } else if (collection_id.contains("folder:")) {
//        url = "https://docs.google.com/feeds/default/private/full/" + QString(collection_id).replace(":", "%3A") + "/contents";
//    } else {
//        qDebug() << "Wrong parent collection_id! No creation done.";
//        return;
//    }
//    QString data = QString(
//                "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
//                "<entry xmlns=\"http://www.w3.org/2005/Atom\">"
//                "    <category scheme=\"http://schemas.google.com/g/2005#kind\""
//                "        term=\"http://schemas.google.com/docs/2007#folder\"/>"
//                "        <title>%1</title>"
//                "</entry>"
//                ).arg(collection_name);
//    QByteArray content = data.toLatin1();

//    request.setUrl(url);

//    qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! url:" << url;

//    request.setRawHeader("GData-Version", "3.0");
//    request.setRawHeader("Authorization", (QString("OAuth %1").arg(access_token)).toLatin1());
//    request.setRawHeader("Content-Type", "application/atom+xml");
//    request.setRawHeader("Content-Length", QString::number(content.size()).toLatin1());

//    PtrRI p(new RequestInfo());
//    p->set_branchPos(0);
//    p->set_action(A_CREATE_NEW_COLLECTION);
//    m_idRequests.append(p);

//    WrapperRI* pRI = new WrapperRI();   //pointer need to be deleted in reply handler because no parent QObject* present
//    pRI->m_pRI = p;
//    request.setOriginatingObject(pRI);

//    qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! content:" << content;


//    QApplication::setOverrideCursor(Qt::WaitCursor);
//    m_pNetworkAccessManager->post(request, content);
}

QString OperationsManager::getFileQuery(const QString& url)
{
    QString query(DELETE_FILE);
    QStringList queryStrs(url.split("/"));
    QString lastParam(queryStrs[queryStrs.count()  - 1]);

    queryStrs = lastParam.split("%3A");
    query += queryStrs[queryStrs.count()  - 1];

    return query;
}

void OperationsManager::slotReplyFinished(QNetworkReply* reply)
{
  emit signalDelFinished();
}


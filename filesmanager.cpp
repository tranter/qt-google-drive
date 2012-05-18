#include "filesmanager.h"
#include <QDebug>

FilesManager::FilesManager(QObject *parent):
    QObject(parent),
    networkAccessManager(NULL),
    parser(NULL)

{
}

void FilesManager::getFiles(const QString& query)
{
    if(networkAccessManager) delete networkAccessManager;
    networkAccessManager = new QNetworkAccessManager(this);

    connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotReplyFinished(QNetworkReply*)));

    CommonTools::setHeader(request);
    request.setUrl(QUrl(query));
    reply = networkAccessManager->get(request);

    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(sslErrors(const QList<QSslError>&)),this, SLOT(slotSslErrors( const QList<QSslError>&)));

    QApplication::setOverrideCursor(Qt::WaitCursor);
}

void FilesManager::slotReplyFinished(QNetworkReply* reply)
{
    qDebug() << "--------------> replyStr" << replyStr;
    //CommonTools::logToFile("currentfiles.txt", replyStr.toAscii());

    if(parseReply(replyStr)) qDebug() << "parse OK";
    else qDebug() << "parse NOT OK";

    replyStr.clear();

    if(!parser->getXMLHandler()->resDownloadingNow()) show();
}

void FilesManager::slotReadyRead()
{
    replyStr.append(reply->readAll());
}

void FilesManager::slotError(QNetworkReply::NetworkError error)
{
    qDebug() << "slotError error:" << error;
}

void FilesManager::slotSslErrors(const QList<QSslError>& errors)
{
    foreach(const QSslError& e,errors)
    {
        qDebug() << "Ssl error:" << e.error();
    }
}

bool FilesManager::parseReply(const QString& str)
{
    QXmlSimpleReader reader;
    QXmlInputSource source;

    if(parser) delete parser;
    parser = new XMLParser(FILE_TYPE);

    connect(parser->getXMLHandler(), SIGNAL(signalAllResDownloaded(int)),this, SLOT(slotResDownloaded(int)));

    source.setData(str.toAscii());

    reader.setContentHandler(parser);
    reader.setErrorHandler(parser);

    return reader.parse(&source);
}

void FilesManager::slotResDownloaded(int queryType)
{
  if(queryType == FILE_TYPE) show();
}

XMLParser* FilesManager::getParser(void) const
{
 return parser;
}

void FilesManager::show(void)
{
    QList<TreeItemInfo::Data> fileItems = parser->getXMLHandler()->getTreeItemInfo()->getFileItems();

    for(int i = 0; i < items.count(); ++i) delete items[i];

    items.clear();

    QApplication::restoreOverrideCursor();

    for(int i = 1; i < fileItems.count(); ++i)
    {
        items.push_back(new QTreeWidgetItem(UiInstance::ui->filesViewWidget));
        items.last()->setText(0, fileItems[i].name);
        items.last()->setIcon(0, QPixmap(fileItems[i].iconPath));



        items.last()->setText(1, fileItems[i].dataOwner);
        items.last()->setText(2, fileItems[i].fileUpdated);
        items.last()->setText(3, fileItems[i].fileSize);
    }
}

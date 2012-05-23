#include "contentmanager.h"
#include <QApplication>
#include <QDebug>

ContentManager::ContentManager(int handleType, QObject *parent):
    NetworkManager(parent),
    type(handleType),
    parser(NULL),
    operationsManager(new OperationsManager)
{
    qDebug() << "ContentManager::ContentManager()";
}

ContentManager::~ContentManager()
{
    qDebug() << "ContentManager::~ContentManager()";
    if(parser) delete parser;
    if(operationsManager) delete operationsManager;
}

void ContentManager::get(const QString& url)
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);

    CommonTools::setHeader(request);
    getRequest(url);
    operationsManager->setNetworkManager(getNetworkManager());

    QApplication::setOverrideCursor(Qt::WaitCursor);
}

void ContentManager::slotReplyFinished(QNetworkReply* reply)
{
    qDebug() << "ContentManager::slotReplyFinished";

    if(parseReply(replyStr)) qDebug() << "parse OK";
    else qDebug() << "parse not OK";

    replyStr.clear();
    if(!parser->getXMLHandler()->resDownloadingNow()) show();

    QApplication::restoreOverrideCursor();
}

bool ContentManager::parseReply(const QString& str)
{
    QXmlSimpleReader reader;
    QXmlInputSource source;

    if(parser) delete parser;
    parser = new XMLParser(type);

    connect(parser->getXMLHandler(), SIGNAL(signalAllResDownloaded(int)),this, SLOT(slotResDownloaded(int)));

    source.setData(str.toAscii());

    reader.setContentHandler(parser);
    reader.setErrorHandler(parser);

    return reader.parse(&source);
}

void ContentManager::slotResDownloaded(int queryType)
{
    qDebug() << "ContentManager::slotResDownloaded";
    if(queryType == type) show();
}

XMLParser* ContentManager::getParser(void) const
{
    return parser;
}

void ContentManager::clear(void)
{
    qDebug() << "ContentManager::clear";
    items.clear();
    if(items.empty()) return;
    for(int i = 0; i < items.count(); ++i) delete items[i];
    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

void ContentManager::deleteFile(const QString &url)
{
  operationsManager->deleteFile(url);
}




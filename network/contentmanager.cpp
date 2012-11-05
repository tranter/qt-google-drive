#include "contentmanager.h"
#include "share/commontools.h"
#include "share/debug.h"
#include <QApplication>

ContentManager::ContentManager(int handleType, QObject *parent):
    NetworkManager(parent),
    opMngr(new OperationsManager(parent)),
    type(handleType)
{
    parser.reset(new XMLParser(type));
}

ContentManager::~ContentManager()
{
}

void ContentManager::get(const QString &url)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    CommonTools::setHeader(request);
    sendRequest(url);
}

void ContentManager::slotReplyFinished(QNetworkReply*)
{
    CommonTools::logToFile(QString::number(type) + ".txt", replyStr.toAscii());

    if(parseReply(replyStr))
    {
        DEBUG << "parse OK";
    }
    else
    {
        DEBUG << "parse not OK";
    }

    replyStr.clear();

    if(!parser->getXMLHandler()->resDownloadingNow())
    {
        show();
    }

    QApplication::restoreOverrideCursor();
}

bool ContentManager::parseReply(const QString &str)
{
    QXmlSimpleReader reader;
    QXmlInputSource source;

    parser.reset(new XMLParser(type));

    connect(parser->getXMLHandler(), SIGNAL(signalAllResDownloaded(int)),this, SLOT(slotResDownloaded(int)));

    source.setData(str.toAscii());

    reader.setContentHandler(parser.data());
    reader.setErrorHandler(parser.data());

    return reader.parse(&source);
}

void ContentManager::slotResDownloaded(int queryType)
{
    if(queryType == type) show();
}

XMLParser* ContentManager::getParser(void) const
{
    return parser.data();
}

void ContentManager::clear(void)
{
    items.clear();

    if(items.empty()) return;

    for(int i = 0; i < items.count(); ++i)
    {
        delete items[i];
    }
}

void ContentManager::del(const QString &url)
{
    opMngr->del(url);
}

void ContentManager::createFolder(const QString &folderUrl, const QString &name)
{
    opMngr->createFolder(folderUrl, name);
}

OperationsManager* ContentManager::getOpMngr(void) const
{
    return opMngr.data();
}




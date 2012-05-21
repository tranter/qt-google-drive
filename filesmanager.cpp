#include "filesmanager.h"
#include <QDebug>

FilesManager::FilesManager(QObject *parent):
    NetworkManager(parent),
    parser(NULL),
    firstRequest(true)

{
}

void FilesManager::getFiles(const QString& url)
{
    if(!firstRequest)
    {
     if(networkManager) delete networkManager;
     networkManager = new QNetworkAccessManager(this);
    }

    CommonTools::setHeader(request);    
    getRequest(url);

    firstRequest = false;
    QApplication::setOverrideCursor(Qt::WaitCursor);
}

void FilesManager::slotReplyFinished(QNetworkReply* reply)
{
    qDebug() << "--------------> replyStr" << replyStr;
    //CommonTools::logToFile("currentfiles.txt", replyStr.toAscii());

    if(parseReply(replyStr)) qDebug() << "parse OK";
    else qDebug() << "parse not OK";

    replyStr.clear();
    if(!parser->getXMLHandler()->resDownloadingNow()) show();
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

    //UiInstance::ui->filesViewWidget->setSortingEnabled(true);
}

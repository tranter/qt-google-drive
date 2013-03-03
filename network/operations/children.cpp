#include "children.h"
#include "settings/settingsmanager.h"
#include "parsers/jsonparser.h"
#include "share/debug.h"
#include "share/commontools.h"

Children::Children(QObject *parent) :
    NetworkManager(parent)
{
}

void Children::fetch(const Items::Data &source)
{
    queries.setRawHeader(SettingsManager().accessToken(), request);
    DEBUG << "SettingsManager().accessToken()" << SettingsManager().accessToken();

    //getRequest(queries.construcChildrenUrl(QString("root")));
    getRequest(queries.fullContent());
}

void Children::slotReplyFinished(QNetworkReply* reply)
{           
    DEBUG << "==========================================================================================================================\n";
    DEBUG << replyStr;
    DEBUG << "==========================================================================================================================\n";

    QString fileName ("output");
    CommonTools::logToFile(fileName, replyStr.toAscii());

    //    JSONParser jParser;
    //    QStringList pathValues;

    //    pathValues << "items" << "parents";
    //DEBUG << "!!!!" << jParser.getParams(replyStr, pathValues, QString("id"));
    //DEBUG << jParser.getParams(replyStr, pathValues, QString("selfLink"));
    //DEBUG << jParser.getParams(replyStr, pathValues, QString("childLink"));

    //updatePanelContent(false);
}


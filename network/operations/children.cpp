#include "children.h"
#include "settings/settingsmanager.h"
#include "parsers/jsonparser.h"
#include "share/debug.h"

Children::Children(QObject *parent) :
    NetworkManager(parent)
{
}

void Children::fetch(const Items::Data &source)
{
    //DEBUG << source.self;
    queries.setRawHeader(SettingsManager().accessToken(), request);
    //getRequest(queries.construcChildrenUrl(QString("root")));0B_pGaTf6anqmMV93Q2FBS1RaT0U
    getRequest(queries.construcChildrenUrl(QString("0B_pGaTf6anqmMV93Q2FBS1RaT0U")));
}

void Children::slotReplyFinished(QNetworkReply* reply)
{           
    DEBUG << "==========================================================================================================================\n";
    DEBUG << replyStr;
    DEBUG << "==========================================================================================================================\n";

    //    JSONParser jParser;
    //    QStringList pathValues;

    //    pathValues << "items" << "parents";
    //DEBUG << "!!!!" << jParser.getParams(replyStr, pathValues, QString("id"));
    //DEBUG << jParser.getParams(replyStr, pathValues, QString("selfLink"));
    //DEBUG << jParser.getParams(replyStr, pathValues, QString("childLink"));

    //updatePanelContent(false);
}


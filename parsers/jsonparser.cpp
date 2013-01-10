#include "jsonparser.h"
#include "share/debug.h"
#include <QRegExp>
#include <QString>
#include <QStringList>

JSONParser::JSONParser()
{
}

QString JSONParser::getValue(const QString &jsonStr, const QString &key)
{
    QString token(getToken(jsonStr, key, QString(",")));

    token.remove(QRegExp("[\"}]"));

    QStringList tokenValues(token.split(": "));
    QString value;

    if(tokenValues.count() == 2) value = tokenValues[1].trimmed();

    return value;
}

QStringList JSONParser::getParams(const QString &jsonStr, const QStringList &pathValues, const QString &key)
{
    if(pathValues.count() == 0)  return QStringList();

    QString token(getToken(jsonStr, pathValues[0], QString("]")));

    for(int i = 1; i < pathValues.count(); ++i)
    {
        token = getToken(token, pathValues[i], QString("]"));
    }

    QStringList tokens;
    QString nextToken;

    continuePos = 0;

    while(!(nextToken = getValue(token, key)).isEmpty())
    {
        token = token.mid(continuePos);
        tokens << nextToken;
    }

    return tokens;
}

QString JSONParser::getToken(const QString &object, const QString &key, const QString &endDivider)
{
    QString searchToken(QString("\"") + key + QString("\""));

    int beginPos(object.indexOf(searchToken));

    if(beginPos == -1) return QString();

    int endPos;

    /*if(endDivider == QString(",")) */endPos = object.indexOf(endDivider, beginPos);
//    else endPos = getTokenEnd(object, beginPos);

    int strLength = endPos - beginPos;
    QString token(object.mid(beginPos, strLength));

    if(endPos != -1)  continuePos = endPos;
    else continuePos = beginPos + token.length();

    DEBUG << "TOKEN" << token;

    return token;
}

int JSONParser::getTokenEnd(const QString &object, int beginPos)
{
    int beginDividerPos(object.indexOf("[", beginPos + 1));
    int endDividerPos(object.indexOf("]", beginPos + 1));

//    while(beginDividerPos !)
//    {
//        DEBUG << "beginDividerPos" << beginDividerPos << " endDividerPos" << endDividerPos;
//        beginDividerPos = object.indexOf("[", endDividerPos);
//        endDividerPos = object.indexOf("]", endDividerPos);
//    }

//    DEBUG << "endDividerPos" << endDividerPos;

    return endDividerPos;
}

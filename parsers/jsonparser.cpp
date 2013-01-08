#include "jsonparser.h"
#include "share/debug.h"
#include <QRegExp>
#include <QString>
#include <QStringList>

JSONParser::JSONParser()
{
}

QString JSONParser::getParam(const QString& jsonStr, const QString& lVal)
{
    QString token(getToken(jsonStr, lVal, QString(",")));

    token.remove(QRegExp("[\"}]"));

    QStringList tokenValues(token.split(": "));
    QString rVal;

    if(tokenValues.count() == 2) rVal = tokenValues[1].trimmed();

    return rVal;
}

QStringList JSONParser::getParams(const QString& jsonStr, const QStringList& pathValues, const QString& lVal)
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

    while(!(nextToken = getParam(token, lVal)).isEmpty())
    {
        token = token.mid(continuePos);
        tokens << nextToken;
    }

    return tokens;
}

QString JSONParser::getToken(const QString& jsonStr, const QString& lVal, QString endDivider)
{
    QString searchToken(QString("\"") + lVal + QString("\""));

    int beginPos(jsonStr.indexOf(searchToken));

    if(beginPos == -1) return QString();

    int endPos(jsonStr.indexOf(endDivider, beginPos));

    int strLength = endPos - beginPos;
    QString token(jsonStr.mid(beginPos, strLength));

    if(endPos != -1)  continuePos = endPos;
    else continuePos = beginPos + token.length();

    return token;
}

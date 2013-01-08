#include "jsonparser.h"
#include "share/debug.h"
#include <QRegExp>
#include <QString>
#include <QStringList>

JSONParser::JSONParser()
{
}

QString JSONParser::getParam(const QString& jsonStr, const QString& lVal, bool isQuotes)
{
    QString token(getToken(jsonStr, lVal, QString(","), isQuotes));
    QStringList tokenValues(token.split(":"));
    QString rVal;

    if(tokenValues.count() == 2) rVal = tokenValues[1].trimmed();

    return rVal;
}

QStringList JSONParser::getParams(const QString& jsonStr, const QStringList& pathValues, const QString& lVal)
{
    if(pathValues.count() == 0)  return QStringList();

    QString token(getToken(jsonStr, pathValues[0], QString("]"), true));

    for(int i = 1; i < pathValues.count(); ++i)
    {
      token = getToken(token, pathValues[i], QString("]"), true);
    }

    continuePos = 0;

    QStringList tokens;

    QString nextToken;

    while(!(nextToken = getParam(token, lVal, false)).isEmpty())
    {
        token = token.mid(continuePos);
        tokens << nextToken;
     }

    return tokens;
}

QString JSONParser::getToken(const QString& jsonStr, const QString& lVal, QString endDivider, bool isQuotes)
{
    QString searchToken(lVal);

    if(isQuotes) searchToken = QString("\"") + lVal + QString("\"");

    int beginPos(jsonStr.indexOf(searchToken));

    if(beginPos == -1) return QString();

    int endPos(jsonStr.indexOf(endDivider, beginPos));
    int strLength = endPos - beginPos;
    QString token(jsonStr.mid(beginPos, strLength));

    token.remove(QRegExp("[\"}]"));

    continuePos = endPos;

    return token;
}

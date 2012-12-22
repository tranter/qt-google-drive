#include "jsonparser.h"
#include <QRegExp>
#include <QString>
#include <QStringList>

JSONParser::JSONParser()
{
}

QString JSONParser::getPlainParam(const QString& jsonStr, const QString& lVal)
{
    QString searchToken(QString("\"") + lVal + QString("\""));
    int beginPos(jsonStr.indexOf(searchToken));

    if(beginPos == -1) return QString();

    int endPos(jsonStr.indexOf(QString(","), beginPos));

    int strLength = endPos - beginPos;
    QString token(jsonStr.mid(beginPos, strLength));

    token.remove(QRegExp("[\"}]"));

    QStringList tokenValues(token.split(": "));
    QString rVal;

    if(tokenValues.count() == 2)
    {
        rVal = tokenValues[1].trimmed();
    }

    return rVal;
}

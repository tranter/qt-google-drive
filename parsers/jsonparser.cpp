#include "jsonparser.h"
#include "share/debug.h"
#include <QRegExp>
#include <QString>
#include <QStringList>

JSONParser::JSONParser()
{
}

QString JSONParser::getPlainParam(const QString& jsonStr, const QString& lVal)
{
    QString optStr(jsonStr);

    optStr.remove(QRegExp("[{}\"\n]"));

    QStringList parseStrs(optStr.split(","));
    QStringList token;
    QString rVal, token0, token1;

    for (int i = 0; i < parseStrs.count(); ++i)
    {
        token = parseStrs[i].split(": ");

        if(token.count() == 2)
        {
            token0 = token[0].trimmed();
            token1 = token[1].trimmed();

            if(token0 == lVal)
            {
                rVal = token1;
                break;
            }
        }
    }

    return rVal;
}

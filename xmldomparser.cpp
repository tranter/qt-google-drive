#include "xmldomparser.h"
#include <QDebug>

XMLDomParser::XMLDomParser(const QString str):
    parseStr(str)
{
}

void XMLDomParser::parseNext(const QDomElement &element)
{
    QDomNode node = element.firstChild();

    qDebug() <<  endl << "---------> tagName: " << element.tagName() << "---------> tag data: " << node.toText().data();

    while (!node.isNull())
    {
        //if(node.toElement().tagName() == "entry") parseNext(node.toElement());

        if (node.toElement().hasChildNodes())
        {
            QDomNode childNode = node.firstChild();

            while (!childNode.isNull())
            {
                qDebug() << "child tag: " << node.toElement().tagName() << "; child node data: " << childNode.toText().data();
                childNode = childNode.nextSibling();
            }
        }

        node = node.nextSibling();
    }
}

void XMLDomParser::parse(void)
{
    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;

    if (!doc.setContent(parseStr, true, &errorStr, &errorLine, &errorColumn))
    {
        qDebug() << "errorLine-" << errorLine << " errorColumn-" << errorColumn << " errorStr" << errorStr.toAscii();
        return;
    }

    QDomElement root = doc.documentElement();

    QDomNode node = root.firstChild();

    while (!node.isNull())
    {
        if(node.toElement().tagName() == "entry")
            parseNext(node.toElement());

        node = node.nextSibling();
    }
}

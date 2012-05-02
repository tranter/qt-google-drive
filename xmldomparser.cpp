#include "xmldomparser.h"
#include <QDebug>

XMLDomParser::XMLDomParser(const QString str):
    parseStr(str)
{
}

void XMLDomParser::parseNext(const QDomElement &element)
{
    QDomNode node = element.firstChild();

    qDebug() <<  "---------> tagName: " << element.tagName() << "data: " << element.toText().data() << endl;

    while (!node.isNull())
    {
        qDebug() << "node - " << node.toText().data();

        if (node.toElement().hasChildNodes())
        {
            QDomNode childNode = node.firstChild();

            while (!childNode.isNull())
            {
                if (childNode.nodeType() == QDomNode::TextNode)
                {
                    qDebug() << "child node tagName: " << childNode.toElement().tagName() << "data: " << childNode.toText().data();
                }
                childNode = childNode.nextSibling();
            }
        }

        qDebug() << "--------------------------------------------------------------------------------------------" << endl;
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
        qDebug() << "Line %d, column %d: %s", errorLine, errorColumn, errorStr.toAscii();
        return;
    }

    QDomElement root = doc.documentElement();

    QDomNode node = root.firstChild();

    while (!node.isNull())
    {
        //if (node.toElement().tagName() == "entry")
        parseNext(node.toElement());

        node = node.nextSibling();
    }
}

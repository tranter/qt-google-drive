#include "xmldomparser.h"
#include <QDebug>

XMLDomParser::XMLDomParser(const QString str):
    parseStr(str)
{
}

void XMLDomParser::parseNext(const QDomElement &element)
{
    QDomNode node = element.firstChild();

    qDebug() <<  endl << "-> tagName: " << element.tagName() << "-> tag name: " << node.nodeName()/* << " parentNode = " << node.parentNode().nodeName()*/;
    //qDebug() << "!!!!!!!!!!!!!!!! " << element.text();


    while (!node.isNull())
    {
        //qDebug() << "------------------------> node tag: " << node.toElement().tagName() << "; node data: " << node.toText().data();

//        if(node.toElement().tagName() == "author")
//        {
//            parseNext(node.toElement());
//        }

        if (node.toElement().hasChildNodes())
        {
            QDomNode childNode = node.firstChild();

            while (!childNode.isNull())
            {
                qDebug() << "in child tag: " << node.toElement().tagName() << "; child node data: " << childNode.toText().data() << " child parentNode = " << childNode.parentNode().nodeName();
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

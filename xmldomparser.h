#ifndef XMLDOMPARSER_H
#define XMLDOMPARSER_H

#include <QDomDocument>
#include <QDomNode>

class XMLDomParser
{
public:
    XMLDomParser(const QString str);

public:
    void parse(void);

private:

  void parseNext(const QDomElement &element);

private:
  QString parseStr;
};

#endif // XMLDOMPARSER_H

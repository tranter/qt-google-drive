#ifndef DEBUG_H
#define DEBUG_H

#include <QDebug>

#define DEBUG qDebug() << "Function:" << __PRETTY_FUNCTION__ << " Line:" << " " << __LINE__

#endif // DEBUG_H

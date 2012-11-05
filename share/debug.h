#ifndef DEBUG_H
#define DEBUG_H

#include <QDebug>

#define DEBUG qDebug() << "[ DEBUG:" << __FUNCTION__ << ", line:" << __LINE__ << "]"

#endif // DEBUG_H

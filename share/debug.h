#ifndef DEBUG_H
#define DEBUG_H

#include <QDebug>

char buffer[512];

#define DEBUG_STR "[DEBUG %s, line: %u] ", __PRETTY_FUNCTION__, __LINE__
#define DEBUG_BUFFER sprintf(buffer, DEBUG_STR)
#define DEBUG(msg, ...) DEBUG_BUFFER; qDebug(strcat(buffer, msg), ##__VA_ARGS__)
#define DEBUG_INFO qDebug(DEBUG_STR)

#endif // DEBUG_H

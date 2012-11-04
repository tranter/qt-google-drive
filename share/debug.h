#ifndef DEBUG_H
#define DEBUG_H

#ifndef QT_NO_DEBUG_OUTPUT

 static char buffer[512];

 #define DEBUG_STR "[DEBUG %s, line: %u] ", __PRETTY_FUNCTION__, __LINE__
 #define DEBUG_BUFFER sprintf(buffer, DEBUG_STR)
 #define DEBUG(msg, ...) DEBUG_BUFFER; qDebug(strcat(buffer, msg), ##__VA_ARGS__)
 #define DEBUG_INFO qDebug(DEBUG_STR)

#else

 #define DEBUG_STR
 #define DEBUG_BUFFER
 #define DEBUG(msg, ...) Q_UNUSED(msg)
 #define DEBUG_INFO

#endif

#endif // DEBUG_H

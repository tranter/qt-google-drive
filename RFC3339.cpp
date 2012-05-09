#include "RFC3339.h"

QDateTime RFC3339::fromString(const QString & dt) {
    return QDateTime::fromString(dt, "yyyy-MM-ddThh:mm:ss.zzzZ");
}

QString   RFC3339::toString(const QDateTime & dt) {
    return dt.toString("yyyy-MM-ddThh:mm:ss.zzzZ");
}

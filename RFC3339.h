#ifndef DATETIME_H
#define DATETIME_H

#include <QDateTime>

namespace RFC3339 {
    QDateTime fromString(const QString & dt);
    QString   toString(const QDateTime & dt);
}

#endif // DATETIME_H

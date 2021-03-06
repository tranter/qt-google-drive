#ifndef MOVE_H
#define MOVE_H

#include <QObject>
#include "network/operations/copy.h"
#include "network/operations/delete.h"

class Move : public QObject, public Operation
{
    Q_OBJECT
public:
    Move(QObject* parent = 0);

public:
    void file(const Items::Data &source, const QString &destination);
    void files(const QList<Items::Data> &sources, const QString &destination);
    const Copy *getCopyPart() const;

private slots:
    void slotFileCopied(Items::Data &source, bool isLastFile);

private:
    Copy copy;
    Delete del;
};

#endif // MOVE_H

#include "move.h"

Move::Move(QObject *parent) :
    QObject(parent),
    Operation(EMove)
{
}

void Move::file(const Items::Data &source, const QString &destFolderUrl)
{
    connect(&copy, SIGNAL(fileCopied(Items::Data&, bool)), this, SLOT(slotFileCopied(Items::Data&, bool)));
    copy.file(source, destFolderUrl);
}

void Move::files(const QList<Items::Data> &sources, const QString &destFolderUrl)
{
    connect(&copy, SIGNAL(fileCopied(Items::Data&, bool)), this, SLOT(slotFileCopied(Items::Data&, bool)));
    copy.files(sources, destFolderUrl);
}

void Move::slotFileCopied(Items::Data &source, bool isLastFile)
{
    del.item(source, isLastFile);
}

const Copy* Move::getCopyPart() const
{
    return &copy;
}

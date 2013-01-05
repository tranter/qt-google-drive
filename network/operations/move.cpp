#include "move.h"

Move::Move(QObject *parent) :
    QObject(parent)
{
}

void Move::file(const Items::Data &source, const QString &destFolderUrl)
{
    copy.file(source, destFolderUrl);
    connect(&copy, SIGNAL(fileCopied(Items::Data&)), this, SLOT(slotFileCopied(Items::Data&)));
}

void Move::slotFileCopied(Items::Data &source)
{
    del.file(source);
}

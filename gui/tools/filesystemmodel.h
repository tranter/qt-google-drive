#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QFileSystemModel>
#include <QStringList>

class FileSystemModel : public QFileSystemModel
{
public:
    FileSystemModel(const QStringList &cn);

public:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

private:
    QStringList columnNames;
};

#endif // FILESYSTEMMODEL_H

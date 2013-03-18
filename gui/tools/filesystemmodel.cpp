#include "filesystemmodel.h"

FileSystemModel::FileSystemModel(const QStringList &cn):
    columnNames(cn)
{   
    setRootPath("");
}

QVariant FileSystemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole || role == Qt::Horizontal) return columnNames[section];

    return QFileSystemModel::headerData(section, orientation, role);
}

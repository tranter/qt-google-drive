#include "fileiconprovider.h"
#include "share/debug.h"
#include <QFile>
#include <QFileInfo>
#include <QApplication>
#include <QDir>


FileIconProvider::FileIconProvider():
    pathWithDot(QDir::toNativeSeparators(QApplication::applicationDirPath() + "/res/iconproviders/provider."))
{
}

QIcon FileIconProvider::getIconByExt(const QString &fileName)
{     
    QString ext(QFileInfo(fileName).suffix());

    if(!ext.isEmpty())
    {
        QFileInfo fileInfo(pathWithDot + ext);
        return icon(fileInfo);
    }

    return QIcon();
}

void FileIconProvider::setExtProviderFile(const QString &fileName)
{
    QString ext(QFileInfo(fileName).suffix());

    if(!ext.isEmpty())
    {
        if(!cashExtProviderFile(pathWithDot + ext)) DEBUG << "error cashing extension provider file " << ext;
    }
}

bool FileIconProvider::cashExtProviderFile(const QString &absoluteFilePath)
{
    bool cashed = true;
    QFile file(absoluteFilePath);

    if(!file.exists())
    {
        if (!file.open(QIODevice::WriteOnly)) cashed = false;
        file.write(" ");
        file.close();
    }

    return cashed;
}


#ifndef FILEICONPROVIDER_H
#define FILEICONPROVIDER_H

#include <QFileIconProvider>

class FileIconProvider : public QFileIconProvider
{
public:
    FileIconProvider();

public:
    QIcon getIconByExt(const QString &fileName);
    void setExtProviderFile(const QString &fileName);

private:
    bool cashExtProviderFile(const QString &fileName);

private:
   QString pathWithDot;
};

#endif // FILEICONPROVIDER_H

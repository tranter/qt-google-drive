#ifndef FILIESTRANSFER_H
#define FILIESTRANSFER_H

#include <QObject>
#include "driveengine.h"

class FiliesTransfer : public QObject
{
    Q_OBJECT
public:
    explicit FiliesTransfer(QObject *parent = 0);

public:
    friend class DriveEngine;

//private slots:
//    void slotDownload(void);
//    void slotUpload(void);

private:
    void download(FilesManager* manager);
    void upload(void);
};

#endif // FILIESTRANSFER_H

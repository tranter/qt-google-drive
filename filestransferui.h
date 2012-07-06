#ifndef FILIESTRANSFER_H
#define FILIESTRANSFER_H

#include "driveengine.h"

class FilesManager;

class FilesTransferUI : public QObject
{
    Q_OBJECT
public:
    explicit FilesTransferUI(QObject *parent = 0);

    friend class DriveEngine;

private slots:
    void slotDownload(void);
    void slotUpload(void);

private:
    void download(FilesManager* manager);
    void upload(void);
};

#endif // FILIESTRANSFER_H

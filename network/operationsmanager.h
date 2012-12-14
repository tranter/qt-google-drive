#ifndef OPERATIONSMANAGER_H
#define OPERATIONSMANAGER_H

#include "network/networkmanager.h"
#include "gui/iteminfo.h"
#include "share/defs.h"

class OperationsManager : public NetworkManager
{
    Q_OBJECT
public:
    enum EOperations
    {
        ENone = -1,
        ECopy,
        ECreateFolder,
        EDelete,
        ERename,
        EShare
    };

    OperationsManager(QObject *parent = 0);

public:
    void deleteFile(const QString &sourceUrl);
    void copyWebFile(const ItemInfo::Data &source, const QString &destFolder);
    void moveWebFile(const ItemInfo::Data &source, const QString &destFolder);
    void renameWebFile(const ItemInfo::Data &source, const QString &newName);
    void shareWebFile(const ItemInfo::Data &source);
    void createFolder(const QString &folderUrl, const QString &name);

protected slots:
    virtual void slotReplyFinished(QNetworkReply *reply);
    virtual void slotPostFinished(QNetworkReply* reply);

private slots:
    void slotPutFinished(void);

protected:
    void setProgressBarSettings(const QString &fileName, const QString &progressBarDialogInfoText);

private:
    QString getIDFromURL(const QString &url);
    QUrl getDeleteFileQuery(const QString &url);
    QUrl getCopyFileQuery(const QString &url);
    void updatePanelContent(bool opposite);

private:
    EOperations currentOperation;
    QString fileURLToDeleteForMoveOperation;
    bool isMove;
};

#endif // OPERATIONSMANAGER_H

#include "settingsmanager.h"
#include "share/debug.h"
#include <QString>

SettingsManager::SettingsManager(QObject *parent) :
    QSettings("ICS", "Google Drive File Manager", parent)
{
}

void SettingsManager::writeAccountInfo(AccountInfo::Data &data)
{
    beginGroup(QString("accounts/") + data.email);

    setValue("name", data.name);
    setValue("data.domainSharingPolicy", data.domainSharingPolicy);
    setValue("data.permissionId", data.permissionId);
    setValue("data.quotaBytesTotal", data.quotaBytesTotal);
    setValue("data.quotaBytesUsed", data.quotaBytesUsed);
    setValue("data.quotaBytesUsedInTrash", data.quotaBytesUsedInTrash);

    endGroup();

    DEBUG << group();
}


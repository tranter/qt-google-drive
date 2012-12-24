#include "settingsmanager.h"
#include "share/defs.h"
#include "share/enums.h"
#include "share/debug.h"
#include <QString>
#include <QStringList>
#include <QDir>

SettingsManager::SettingsManager(QObject *parent) :
    QSettings("ICS", "Google Drive File Manager", parent)
{
}

void SettingsManager::writeAccountInfo(AccountInfo::Data &data)
{
    beginGroup(ACCOUNTS_GROUP + QString("/") + data.email);

    setValue(NAME_KEY, data.name);
    setValue(DOMAIN_SHARING_POLICY_KEY, data.domainSharingPolicy);
    setValue(PERMISSION_ID_KEY, data.permissionId);
    setValue(QUOTA_BYTES_TOTAL_KEY, data.quotaBytesTotal);
    setValue(QUOTA_BYTES_USED_KEY, data.quotaBytesUsed);
    setValue(QUOTA_BYTES_USED_IN_TRASH_KEY, data.quotaBytesUsedInTrash);
    setValue(ACCESS_TOKEN_KEY, data.accessToken);

    if(!data.refreshToken.isEmpty())
    {
        setValue(REFRESH_TOKEN_KEY, data.refreshToken);
    }

    endGroup();
}

int SettingsManager::currentPanel(void)
{
    int num;

    QString currentPanelVal = getValueFromGroup(COMMON_GROUP, CURRENT_PANEL_KEY, LEFT_PANEL_VALUE).toString();

    if(currentPanelVal == LEFT_PANEL_VALUE) num = 0;
    if(currentPanelVal == RIGHT_PANEL_VALUE) num = 1;

    return num;
}

void SettingsManager::setCurrentPanel(const QString &panelName)
{
    setValueInGroup(COMMON_GROUP, CURRENT_PANEL_KEY, panelName);
}

void SettingsManager::setInitialLoading(bool initLoad)
{
    setValueInGroup(COMMON_GROUP, INIT_LOAD_KEY, initLoad);
}

bool SettingsManager::initialLoading(void)
{
    return getValueFromGroup(COMMON_GROUP, INIT_LOAD_KEY, false).toBool();
}

void SettingsManager::setCurrentFolderPath(int panelNum, const QString &path)
{
    setValueInPanelGroup(panelNum, CURRENT_FOLDER_PATH_KEY, path);
}

QString SettingsManager::currentFolderPath(int panelNum)
{
    return getValueFromPanelGroup(panelNum, CURRENT_FOLDER_PATH_KEY, QString("a:") + QDir::toNativeSeparators("/")).toString();
}

void SettingsManager::setCurrentFolderURL(int panelNum, const QString &url)
{
    setValueInPanelGroup(panelNum, CURRENT_FOLDER_URL_KEY, url);
}

QString SettingsManager::currentFolderURL(int panelNum)
{
    return getValueFromPanelGroup(panelNum, CURRENT_FOLDER_URL_KEY, GET_FULL_ROOT_CONTENT).toString();
}

void SettingsManager::setPathesURLs(int panelNum, QStringList pathes)
{
    setValueInPanelGroup(panelNum, PATHES_URLS_KEY, pathes);
}

QStringList SettingsManager::pathesURLs(int panelNum)
{
    return getValueFromPanelGroup(panelNum, PATHES_URLS_KEY).toStringList();
}

bool SettingsManager::isWorkDirSet(void)
{
    return exists(COMMON_GROUP, WORK_DIR_KEY);
}

QString SettingsManager::workDir(void)
{
    return getValueFromGroup(COMMON_GROUP, WORK_DIR_KEY).toString();
}

void SettingsManager::setWorkDir(const QString &workDrName)
{
    setValueInGroup(COMMON_GROUP, WORK_DIR_KEY, workDrName);
}

QString SettingsManager::accessToken(void)
{
    QString accountName(currentAccount(currentPanel()));
    return getValueFromGroup(ACCOUNTS_GROUP + QString("/") + accountName, ACCESS_TOKEN_KEY).toString();
}

QString SettingsManager::refreshToken(void)
{
    QString accountName(currentAccount(currentPanel()));
    return getValueFromGroup(ACCOUNTS_GROUP + QString("/") + accountName, REFRESH_TOKEN_KEY).toString();
}

void SettingsManager::setCurrentAccount(int panelNum, const QString &name)
{
    setValueInPanelGroup(panelNum, CURRENT_ACCOUNT_KEY, name);
}

QString SettingsManager::currentAccount(int panelNum)
{
    return getValueFromPanelGroup(panelNum, CURRENT_ACCOUNT_KEY).toString();
}

bool SettingsManager::isAnyAccount(void)
{
    beginGroup(ACCOUNTS_GROUP);
    bool is = childGroups().count() > 0;
    endGroup();

    return is;
}

void SettingsManager::setValueInPanelGroup(int panelNum, const QString &key, const QVariant &val)
{
    beginGroup(PANEL_GROUP + QString::number(panelNum));
    setValue(key, val);
    endGroup();
}

QVariant SettingsManager::getValueFromPanelGroup(int panelNum, const QString &key, const QVariant &defaultVal)
{
    beginGroup(PANEL_GROUP + QString::number(panelNum));
    QVariant val(value(key, defaultVal));
    endGroup();

    return val;
}

void SettingsManager::setValueInGroup(const QString &group, const QString &key, const QVariant &val)
{
    beginGroup(group);
    setValue(key, val);
    endGroup();
}

QVariant SettingsManager::getValueFromGroup(const QString &group, const QString &key, const QVariant &defaultVal)
{
    beginGroup(group);
    QVariant val(value(key, defaultVal));
    endGroup();

    return val;
}

bool SettingsManager::exists(const QString &group, const QString &key)
{
    beginGroup(group);
    bool is =  contains(WORK_DIR_KEY);
    endGroup();

    return is;
}





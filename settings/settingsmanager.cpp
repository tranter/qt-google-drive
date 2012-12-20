#include "settingsmanager.h"
#include "share/defs.h"
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
    beginGroup(QString("accounts/") + data.email);

    setValue("name", data.name);
    setValue("domainSharingPolicy", data.domainSharingPolicy);
    setValue("permissionId", data.permissionId);
    setValue("quotaBytesTotal", data.quotaBytesTotal);
    setValue("quotaBytesUsed", data.quotaBytesUsed);
    setValue("quotaBytesUsedInTrash", data.quotaBytesUsedInTrash);

    endGroup();
}

QString SettingsManager::currentPanel(void) const
{
    return value(CURRENT_PANEL, LEFT_PANEL).toString();
}

void SettingsManager::setCurrentPanel(const QString &panelName)
{
    setValue(CURRENT_PANEL, panelName);
}

void SettingsManager::setInitialLoading(bool initLoad)
{
    setValue(INIT_LOAD, initLoad);
}

void SettingsManager::setCurrentFolderPath(int panelNum, const QString &path)
{
    //    beginGroup(PANEL + QString::number(panelNum));
    //    setValue(CURRENT_FOLDER_PATH, path);
    //    endGroup();
    setStrValueInPanelGroup(panelNum, CURRENT_FOLDER_PATH, path);
}

QString SettingsManager::currentFolderPath(int panelNum)
{
    //    beginGroup(PANEL + QString::number(panelNum));
    //    QString val(value(CURRENT_FOLDER_PATH, QString("a:") + QDir::toNativeSeparators("/")).toString());
    //    endGroup();
    //    return val;

    return getStrValueFromPanelGroup(panelNum, CURRENT_FOLDER_PATH, QString("a:") + QDir::toNativeSeparators("/"));
}

void SettingsManager::setCurrentFolderURL(int panelNum, const QString &url)
{
    //    beginGroup(PANEL + QString::number(panelNum));
    //    setValue(CURRENT_FOLDER_URL, url);
    //    endGroup();

    setStrValueInPanelGroup(panelNum, CURRENT_FOLDER_URL, url);
}

QString SettingsManager::currentFolderURL(int panelNum)
{
//    beginGroup(PANEL + QString::number(panelNum));
//    QString val(value(CURRENT_FOLDER_URL, GET_FULL_ROOT_CONTENT).toString());
//    endGroup();

//    return val;

    return getStrValueFromPanelGroup(panelNum, CURRENT_FOLDER_URL, GET_FULL_ROOT_CONTENT);
}

void SettingsManager::setPathesURLs(int panelNum, QStringList pathes)
{
    beginGroup(PANEL + QString::number(panelNum));
    setValue(PATHES_URLS, pathes);
    endGroup();
}

QStringList SettingsManager::pathesURLs(int panelNum)
{
    beginGroup(PANEL + QString::number(panelNum));
    QStringList val(value(PATHES_URLS).toStringList());
    endGroup();

    return val;
}

bool SettingsManager::isWorkDirSet(void)
{
    return contains(WORK_DIR);
}

QString SettingsManager::workDir(void) const
{
    return value(WORK_DIR).toString();
}

void SettingsManager::setWorkDir(const QString &workDrName)
{
    setValue(WORK_DIR, workDrName);
}

void SettingsManager::setAccessToken(const QString &accessToken)
{
    setValue(ACCESS_TOKEN, accessToken);
}

QString SettingsManager::accessToken(void) const
{
    return value(ACCESS_TOKEN).toString();
}

void SettingsManager::setRefreshToken(const QString &refreshToken)
{
    setValue(REFRESH_TOKEN, refreshToken);
}

QString SettingsManager::refreshToken(void) const
{
    return value(REFRESH_TOKEN).toString();
}

void SettingsManager::setStrValueInPanelGroup(int panelNum, const QString &key, const QString &val)
{
    beginGroup(PANEL + QString::number(panelNum));
    setValue(key, val);
    endGroup();
}

QString SettingsManager::getStrValueFromPanelGroup(int panelNum, const QString &key, const QString &defaultVal)
{
    beginGroup(PANEL + QString::number(panelNum));
    QString val(value(key, defaultVal).toString());
    endGroup();

    return val;
}


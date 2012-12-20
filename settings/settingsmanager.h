#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include "settings/accountinfo.h"
#include <QSettings>

class SettingsManager : public QSettings
{
    Q_OBJECT
public:
    explicit SettingsManager(QObject *parent = 0);

public:
    void writeAccountInfo(AccountInfo::Data &data);
    QString currentPanel(void) const;
    void setCurrentPanel(const QString &panelName);
    void setInitialLoading(bool initLoad);
    void setCurrentFolderPath(int panelNum, const QString &path);
    QString currentFolderPath(int panelNum);
    void setCurrentFolderURL(int panelNum, const QString &url);
    QString currentFolderURL(int panelNum);
    void setPathesURLs(int panelNum, QStringList pathes);
    QStringList pathesURLs(int panelNum);
    bool isWorkDirSet(void);
    QString workDir(void) const;
    void setWorkDir(const QString &workDrName);
    void setAccessToken(const QString &accessToken);
    QString accessToken(void) const;
    void setRefreshToken(const QString &refreshToken);
    QString refreshToken(void) const;

private:
    void setStrValueInPanelGroup(int panelNum, const QString &key, const QString &val);
    QString getStrValueFromPanelGroup(int panelNum, const QString &key, const QString &defaultVal);
};

#endif // SETTINGSMANAGER_H

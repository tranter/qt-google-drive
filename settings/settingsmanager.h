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
};

#endif // SETTINGSMANAGER_H

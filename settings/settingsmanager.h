#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QSettings>

class SettingsManager : public QSettings
{
    Q_OBJECT
public:
    explicit SettingsManager(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // SETTINGSMANAGER_H

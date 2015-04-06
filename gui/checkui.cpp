#include "checkui.h"
#include "settings/settingsmanager.h"
#include <QMessageBox>
#include "gui/forms/settingsdialog.h"
#include "share/defs.h"
#include "share/registration.h"
#include "gui/forms/mainwindow.h"
#include "ui_mainwindow.h"
#include "core/driveengine.h"

CheckUI::CheckUI(QObject *parent) :
    QObject(parent)
{
}

bool CheckUI::checkReg(void)
{
    bool regState = true;

    SettingsManager settingsManager;
    QString id, secret, uri;
    id =  settingsManager.getValueFromGroup(COMMON_GROUP, CLIENT_ID_KEY, "").toString();
    secret =  settingsManager.getValueFromGroup(COMMON_GROUP, CLIENT_SECRET_KEY, "").toString();
    uri =  settingsManager.getValueFromGroup(COMMON_GROUP, REDIRECT_URI_KEY, "").toString();


    if(id == "" || secret == "" || uri == "")
    {
        regState = false;
    }

    return regState;
}


bool CheckUI::slotCheckWorkDir(bool showDlg)
{   
    SettingsManager settingsManager;

    if(settingsManager.isWorkDirSet() && !showDlg)
    {
        return true;
    }

    bool dirTextNotEmpty = false;

    if(showDlg)
    {
        SettingsDialog dlg(SDriveEngine::inst()->getParent());

        dlg.setDirectoryPath(settingsManager.workDir());

        QString id, secret, uri;
        id =  settingsManager.getValueFromGroup(COMMON_GROUP, CLIENT_ID_KEY, "").toString();
        secret =  settingsManager.getValueFromGroup(COMMON_GROUP, CLIENT_SECRET_KEY, "").toString();
        uri =  settingsManager.getValueFromGroup(COMMON_GROUP, REDIRECT_URI_KEY, "").toString();

        dlg.setAppInformation(id, secret, uri);


        if(dlg.exec() == QDialog::Accepted)
        {
            if(!dlg.directoryPath().isEmpty() )
            {
                settingsManager.setWorkDir(dlg.directoryPath());
                dirTextNotEmpty = true;
            }
            dlg.getAppInformation(id, secret, uri);
            settingsManager.setValueInGroup(COMMON_GROUP, CLIENT_ID_KEY, id);
            settingsManager.setValueInGroup(COMMON_GROUP, CLIENT_SECRET_KEY, secret);
            settingsManager.setValueInGroup(COMMON_GROUP, REDIRECT_URI_KEY, uri);
        }
    }

    return dirTextNotEmpty;
}

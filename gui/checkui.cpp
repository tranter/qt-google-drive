#include "checkui.h"
#include "settings/settingsmanager.h"
#include <QMessageBox>
#include "gui/forms/settingsdialog.h"
#include "share/defs.h"
#include "gui/forms/mainwindow.h"
#include "ui_mainwindow.h"
#include "core/driveengine.h"

CheckUI::CheckUI(QObject *parent) :
    QObject(parent)
{
}

bool CheckUI::checkReg(void)
{
    return false;
}

bool CheckUI::slotCheckWorkDir(bool showDlg)
{   
    SettingsManager settingsManager;

    if(settingsManager.isWorkDirSet() && !showDlg)
    {
        return true;
    }

    bool dirTextIsNotEmpty = false;

    if(showDlg)
    {
        SettingsDialog settingsDialog(SDriveEngine::inst()->getParent());

        settingsDialog.setDirectoryPath(settingsManager.workDir());

        switch(settingsDialog.exec())
        {
        case QDialog::Accepted:
        {
            if(!settingsDialog.directoryPath().isEmpty() )
            {
                settingsManager.setWorkDir(settingsDialog.directoryPath());
                dirTextIsNotEmpty = true;
            }

        }
            break;
        }
    }

    return dirTextIsNotEmpty;
}

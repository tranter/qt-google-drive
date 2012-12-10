#include "checkui.h"
#include <QSettings>
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

    if(CLIENT_ID == QString("YOUR_CLIENT_ID_HERE") || REDIRECT_URI == QString("YOUR_REDIRECT_URI_HERE") || CLIENT_SECRET == QString("YOUR_CLIENT_SECRET"))
    {
        regState = false;
    }

    return regState;
}


bool CheckUI::slotCheckWorkDir(bool showDlg)
{   
    QSettings settings(COMPANY_NAME, APP_NAME);

    if(settings.contains(WORK_DIR) && !showDlg)
    {
        return true;
    }

    bool dirTextNotEmpty = false;

    if(settings.contains(WORK_DIR) && showDlg)
    {
        SettingsDialog dlg(SDriveEngine::inst()->getParent());


        dlg.setDirectoryPath(settings.value(WORK_DIR).toString());

        switch(dlg.exec())
        {
        case QDialog::Accepted:
        {
            if(!dlg.directoryPath().isEmpty() )
            {
                settings.setValue(WORK_DIR,dlg.directoryPath());
                dirTextNotEmpty = true;
            }

        }
            break;
        }
    }

    return dirTextNotEmpty;
}

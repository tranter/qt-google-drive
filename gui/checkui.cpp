#include "checkui.h"
#include <QSettings>
#include <QMessageBox>
#include "settingsdialog.h"
#include "Def.h"
#include "AppRegData.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "driveengine.h"

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

        // TODO: change link to wiki page
        QMessageBox::warning(SDriveEngine::inst()->getParent(), "Warning",
                             "To work with application you need to register your own application in <b>Google</b>.\n"
                             "Learn more from <a href='http://code.google.com/p/qt-google-drive/'>here</a>");
    }

    return regState;
}


bool CheckUI::slotCheckWorkDir(bool showDlg)
{
    QSettings settings(COMPANY_NAME, APP_NAME);
    SettingsDialog dlg(SDriveEngine::inst()->getParent());
    bool dirTextNotEmpty = false;

    if(settings.contains(WORK_DIR) && showDlg)
    {
        dlg.setDirectoryPath(settings.value(WORK_DIR).toString());
    }
    else if(settings.contains(WORK_DIR) && !showDlg)
    {
        return true;
    }

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

    return dirTextNotEmpty;
}

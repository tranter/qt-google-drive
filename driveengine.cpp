#include "driveengine.h"
#include <QDebug>
#include <QSettings>
#include "AppRegData.h"
#include <QMessageBox>

DriveEngine::DriveEngine(QObject *parent) :
    QObject(parent),
    createFolderDialog(NULL)
{
    this->parent = static_cast<QWidget*>(parent);
    for(int i = 0; i < EElementsStatesCount; ++i) elementsStates[i] = false;
}

DriveEngine::~DriveEngine()
{
}

void DriveEngine::slotStartLogin()
{
    oAuth2->startLogin(false);
}

void DriveEngine::slotStartLoginFromMenu()
{
    oAuth2->startLogin(true);
}

void DriveEngine::init(void)
{
    additionalFilesManager.reset(new AdditionalFoldersManager);
    filesManager.reset(new FilesManager);
    filesTransfer.reset(new FilesTransferUI);
    filesUI.reset(new FilesUI);
    foldersUI.reset(new FoldersUI);
    oAuth2.reset(new OAuth2(parent));
    operationsUI.reset(new OperationsUI);

    setConnections();

    foldersUI->showFolders();
    foldersUI->showAdditionalFolders();

    SUi::inst()->filesView->header()->resizeSection(0, 380);
}

void DriveEngine::setConnections(void)
{
    connect(SUi::inst()->foldersView, SIGNAL(clicked (const QModelIndex&)), foldersUI.data(), SLOT(slotFoldersViewClicked(const QModelIndex&)));
    connect(SUi::inst()->filesView, SIGNAL(clicked (const QModelIndex&)), filesUI.data(), SLOT(slotFilesViewClicked(const QModelIndex&)));
    connect(SUi::inst()->additionalFoldersView, SIGNAL(clicked (const QModelIndex&)), filesUI.data(), SLOT(slotAdditionalShowFiles(const QModelIndex&)));
    connect(SUi::inst()->filesView->header(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), filesUI.data(), SLOT(slotFilesSortIndicatorChanged(int, Qt::SortOrder)));
}

bool DriveEngine::slotCheckWorkDir(bool showDlg)
{
    QSettings settings(COMPANY_NAME, APP_NAME);
    SettingsDialog dlg(parent);
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

FilesTransferUI* DriveEngine::getfilesTransferUI(void) const
{
    return filesTransfer.data();
}

FoldersManager* DriveEngine::getFoldersManager(void) const
{
    return foldersManager.data();
}

OAuth2* DriveEngine::getOAuth2(void) const
{
    return oAuth2.data();
}

OperationsUI* DriveEngine::getOperationsUI(void) const
{
    return operationsUI.data();
}

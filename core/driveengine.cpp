#include "driveengine.h"
#include "share/debug.h"
#include "settings/settingsmanager.h"
#include "gui/controls/spacer.h"
#include <QMessageBox>
#include <QHBoxLayout>
#include <QSplitter>
#include <QDir>
#include <QPushButton>
#include <QPicture>

DriveEngine::DriveEngine(QWidget *p) :
    parent(p)
{
}

DriveEngine::~DriveEngine()
{
}

void DriveEngine::init()
{
    SUi::inst()->mainToolBar->addWidget(new Spacer(SUi::inst()->centralWidget));
    SUi::inst()->mainToolBar->addAction(SUi::inst()->actionAbout);
    SUi::inst()->shareButton->setVisible(false);

    QHBoxLayout *hBoxLayout = new QHBoxLayout(SUi::inst()->panelsWidget);
    QSplitter *hSplitter = new QSplitter(Qt::Horizontal, SUi::inst()->panelsWidget);

    filePanels[ELeft] = new FilePanel(ELeft);
    filePanels[ERight] = new FilePanel(ERight);

    hBoxLayout->setContentsMargins(0, 0, 0, 0);
    hBoxLayout->addWidget(hSplitter);

    hSplitter->setHandleWidth(1);

    hSplitter->addWidget(filePanels[ELeft]);
    hSplitter->addWidget(filePanels[ERight]);

    for(int i = 0; i < EPanelsCount; ++i)
    {
        contentMngr[i].reset(new ContentManager);
        contentMngr[i]->init();
    }

    checkUI.reset(new CheckUI);
    filesTransferUI.reset(new FilesTransferUI);
    contentUI.reset(new ContentUI);

    if(SettingsManager().isAnyAccount()) updatePanel(ELeft, true);
    if(!SettingsManager().isWorkDirSet()) checkUI->slotCheckWorkDir(true);
}

FilePanel* DriveEngine::getFilePanel(int panel) const
{
    return filePanels[panel];
}

ContentManager* DriveEngine::getContentMngr(bool opposite) const
{
    ContentManager* cm = NULL;
    EPanels currentPanel = static_cast<EPanels> (SettingsManager().currentPanel());

    if(currentPanel == ELeft)
    {
        if(opposite) cm = contentMngr[ERight].data();
        else cm = contentMngr[ELeft].data();
    }

    if(currentPanel == ERight)
    {
        if(opposite) cm = contentMngr[ELeft].data();
        else cm = contentMngr[ERight].data();
    }

    return cm;
}

void DriveEngine::updatePanel(int panelNum, bool initLoad)
{  
    SettingsManager().setInitialLoading(initLoad);
    filePanels[panelNum]->update();
}

bool DriveEngine::isPanelsContentIdentical()
{
    SettingsManager settingsManager;
    bool is = false;

    if(settingsManager.currentAccount(0) == settingsManager.currentAccount(1))
    {
      if(settingsManager.currentFolderURL(0) == settingsManager.currentFolderURL(1))
      {
         is = true;
      }
    }

    DEBUG << is;

    return is;
}

void DriveEngine::slotFirstPanelIsLoaded()
{
    updatePanel(ERight, false);
}

CheckUI* DriveEngine::getCheckUI() const
{
    return checkUI.data();
}

ContentUI* DriveEngine::getContentUI() const
{
    return contentUI.data();
}

FilesTransferUI* DriveEngine::getfilesTransferUI() const
{
    return filesTransferUI.data();
}

QWidget* DriveEngine::getParent() const
{
    return parent;
}

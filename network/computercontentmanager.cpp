#include <QFileInfoList>
#include "computercontentmanager.h"
#include "settings/settingsmanager.h"
#include "share/debug.h"

ComputerContentManager::ComputerContentManager(FilePanel *fp, QObject *parent):
    ContentManager(fp)
{
}

ComputerContentManager::~ComputerContentManager()
{
}

void ComputerContentManager::get(const QString &resourcePointer)
{
    SettingsManager().setCurrentFolderComputerPath(panelNum, resourcePointer);

    dir.setPath(resourcePointer);
    dir.setFilter(QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::DirsFirst | QDir::Name);
}

void ComputerContentManager::addItem(QFileInfo &fileInfo)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(panel);
    QDateTime fileDateTime(fileInfo.lastModified());

    item->setText(0, fileInfo.fileName());
    item->setIcon(0, fileIconProvider.icon(fileInfo));
    item->setText(1, fileInfo.owner());
    item->setText(2, getDate(fileDateTime));
    item->setText(3, getSize(fileInfo.size()));
}

bool ComputerContentManager::isRootFolder()
{
    return dir.isRoot();
}

QString ComputerContentManager::getParentFolder() const
{
}

QString ComputerContentManager::back()
{
}

void ComputerContentManager::update()
{
    get(SettingsManager().currentFolderComputerPath(panelNum));
    show();
}

void ComputerContentManager::accountsComboBoxItemActivated(const QString &text)
{
}

void ComputerContentManager::showFilesOnPanel(QTreeWidgetItem *item)
{    
  QFileInfoList list = dir.entryInfoList();

  get(list.at(item->treeWidget()->currentIndex().row()).absoluteFilePath());
  show();
}

void ComputerContentManager::show()
{
    ContentManager::show();

    QFileInfoList list = dir.entryInfoList();
    //QFileInfoList list = dir.drives();

    for (int i = 0; i < list.size(); ++i)
    {    
        QFileInfo fileInfo = list.at(i);
        addItem(fileInfo);
    }
}

void ComputerContentManager::updateItemsState()
{
 DEBUG;
}

void ComputerContentManager::fillComboBox(QMap<QString, QString> accountsMap, const QString &currentAccount)
{
    DEBUG;
}


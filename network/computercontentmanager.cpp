#include <QFileInfoList>
#include "computercontentmanager.h"
#include "settings/settingsmanager.h"
#include "share/debug.h"

ComputerContentManager::ComputerContentManager(FilePanel *fp, QObject *parent):
    ContentManager(fp)
{
     pathes = SettingsManager().computerPathes(panelNum);
}

ComputerContentManager::~ComputerContentManager()
{
}

void ComputerContentManager::get(const QString &resourcePointer)
{
    SettingsManager().setCurrentFolderComputerPath(panelNum, resourcePointer);

    addPath(resourcePointer);
    SettingsManager().setComputerPathes(panelNum, pathes);

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

bool ComputerContentManager::isRoot()
{
    return dir.isRoot();
}

QString ComputerContentManager::getParentFolder() const
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
    if(hasItemParentSign(item))
    {
        get(back());
        show();
    }
    else
    {
        QFileInfoList list = dir.entryInfoList();
        QFileInfo fileInfo;

        int index = item->treeWidget()->currentIndex().row();
        if(!isRoot()) --index;

        fileInfo = list.at(index);

        if(fileInfo.isDir())
        {
            get(fileInfo.absoluteFilePath());
            show();
        }
    }
}

void ComputerContentManager::show()
{
    ContentManager::show();

    QFileInfoList list = dir.entryInfoList();

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


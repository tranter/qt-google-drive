#include <QFileInfoList>
#include <QHeaderView>
#include "computercontentmanager.h"
#include "settings/settingsmanager.h"
#include "share/defs.h"
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

    pathLabel->setText(QDir::toNativeSeparators(resourcePointer).toLower());

    dir.setPath(resourcePointer);
    dir.setFilter(QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoSymLinks | QDir::NoDotAndDotDot);
}

void ComputerContentManager::addItem(QFileInfo &fileInfo)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(panel);
    QDateTime fileDateTime = fileInfo.lastModified();

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

void ComputerContentManager::update()
{
    DEBUG;

    SettingsManager settingsManager;

    get(settingsManager.currentFolderComputerPath(panelNum));
    show();

    ComboBoxItem drivesMap;
    QString currentDrive = settingsManager.currentComputerDrive(panelNum);
    QFileInfoList list = QDir().drives();

    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        QMap<QString, QIcon> drivesAdditionalInfo;
        QString driveLetter (makeRootDrive(fileInfo.absoluteFilePath()));

        drivesAdditionalInfo[QString()] = fileIconProvider.icon(fileInfo);
        drivesMap[driveLetter] = drivesAdditionalInfo;
    }

    fillComboBox(drivesMap, currentDrive);
}

void ComputerContentManager::accountsComboBoxItemActivated(const QString &text)
{
    QString driveLetter = text.trimmed();
    driveLetter = driveLetter[0];

    SettingsManager settingsManager;

    if(settingsManager.currentComputerDrive(panelNum) != driveLetter)
    {
        settingsManager.setCurrentComputerDrive(panelNum, driveLetter);
        get(makeRootPath(driveLetter));
        show();
    }
}

QString ComputerContentManager::makeRootPath(const QString &driveLetter)
{
    return driveLetter + ":/";
}

QString ComputerContentManager::makeRootDrive(const QString &drive)
{
    QString driveLetter = drive;

    driveLetter.truncate(driveLetter.length() - 2);
    driveLetter = driveLetter.toLower();

    return driveLetter;
}

void ComputerContentManager::showFilesOnPanel(QTreeWidgetItem *item)
{
    DEBUG;

    if(hasItemParentSign(item))
    {
        get(parentFolder());
        show();
    }
    else
    {
        QFileInfo fileInfo;
        QFileInfoList list = dir.entryInfoList();

        int index = item->treeWidget()->currentIndex().row();
        if(!isRoot()) --index;

        fileInfo = list.at(index);

        if(fileInfo.isDir())
        {
            QString absoluteFilePath = fileInfo.absoluteFilePath();

            addPath(absoluteFilePath);
            get(absoluteFilePath);
            show();
        }
    }

    SettingsManager().setComputerPathes(panelNum, pathes);
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

void ComputerContentManager::updateItemsState(QByteArray &values)
{    
    if(values.isEmpty())
    {
        dir.setSorting(QDir::Name | QDir::DirsFirst);
        panel->header()->setSortIndicator(0, Qt:: AscendingOrder);
    }
    else
    {
        panel->header()->restoreState(values);
        setSorting(panel->header()->sortIndicatorSection(), panel->header()->sortIndicatorOrder());
    }
}

void ComputerContentManager::setSorting(int sortSection, Qt::SortOrder sortOrder)
{
    QDir::SortFlags sortFlags;

    switch(sortSection)
    {
    case 0: sortFlags = QDir::Name  | QDir::DirsFirst;
        break;
    case 2: sortFlags = QDir::Time | QDir::DirsFirst;
        break;
    case 3: sortFlags = QDir::Size | QDir::DirsFirst;
        break;
    default: sortFlags = QDir::Name | QDir::DirsFirst;
    }

    if(sortOrder == Qt::AscendingOrder) dir.setSorting(sortFlags);
    else dir.setSorting(sortFlags | QDir::Reversed);
}

QString ComputerContentManager::parentFolder()
{
    SettingsManager settingsManager;

    QString currentFolderComputerPath = settingsManager.currentFolderComputerPath(panelNum);
    QDir dir = currentFolderComputerPath;
    QString parentFolderPath;

    QFileInfoList list = dir.entryInfoList();

    if(!list.isEmpty()) parentFolderPath = list.at(0).absolutePath();
    if(parentFolderPath.isEmpty()) parentFolderPath = makeRootPath(settingsManager.currentComputerDrive(panelNum));

    return parentFolderPath;
}


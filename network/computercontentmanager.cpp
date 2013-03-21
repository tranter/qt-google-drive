#include "computercontentmanager.h"

ComputerContentManager::ComputerContentManager(FilePanel *fp, QObject *parent):
    ContentManager(fp)
{
}

ComputerContentManager::~ComputerContentManager()
{
}

void ComputerContentManager::get(const QString &resourcePointer)
{
}

QString ComputerContentManager::getParentFolder() const
{
}

QString ComputerContentManager::back()
{
}

void ComputerContentManager::update()
{
}

void ComputerContentManager::accountsComboBoxItemActivated(const QString &text)
{
}

void ComputerContentManager::showFilesOnPanel(QTreeWidgetItem *item)
{
}

void ComputerContentManager::show()
{
}

void ComputerContentManager::updateItemsState()
{

}

void ComputerContentManager::fillAcountsComboBox(QMap<QString, QString> accountsMap, const QString &currentAccount)
{
}


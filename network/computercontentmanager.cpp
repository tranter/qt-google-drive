#include "computercontentmanager.h"


ComputerContentManager::ComputerContentManager(QObject *parent)
{
}

ComputerContentManager::~ComputerContentManager()
{
//    if(panel && !panel->topLevelItemCount() > 0) panel->clear();
//    pathesURLs.clear();
}

void ComputerContentManager::show(void)
{
}

void ComputerContentManager::updateItemsState(void)
{
//    Items::Data::ESortOrder itemSortOrder;
//    Qt::SortOrder sortOrder;

//    QByteArray values(SettingsManager().restorePanelHeaderState(panelNum));

//    if(values.isEmpty())
//    {
//        itemSortOrder = Items::Data::ETypeName;
//        sortOrder = Qt::AscendingOrder;

//        panel->header()->setSortIndicator(0, sortOrder);
//    }
//    else
//    {
//        panel->header()->restoreState(values);

//        itemSortOrder = static_cast<Items::Data::ESortOrder> (panel->header()->sortIndicatorSection());
//        sortOrder =  static_cast<Qt::SortOrder> (panel->header()->sortIndicatorOrder());
//    }

//    setItems(itemSortOrder, sortOrder);
}


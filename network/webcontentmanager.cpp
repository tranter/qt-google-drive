#include <QHeaderView>
#include <QXmlSimpleReader>
#include "webcontentmanager.h"
#include "share/commontools.h"
#include "settings/settingsmanager.h"
#include "core/driveengine.h"
#include "gui/tools/fileiconprovider.h"
#include "share/debug.h"

WebContentManager::WebContentManager(QTreeWidget *p, int pn, QObject *parent) :
    ContentManager(p, pn)
{
     parser.reset(new XMLParser);
}

WebContentManager::~WebContentManager()
{
    pathesURLs.clear();
}

void WebContentManager::get(const QString &url)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    CommonTools::setHeader(SettingsManager().accessToken(), request);

    getRequest(url);
}

void WebContentManager::slotReplyFinished(QNetworkReply* reply)
{
    //    CommonTools::logToFile(QString("ParserReply ") + ".txt", replyStr.toAscii());

    //    DEBUG << "<===============================================================================================================";
    //    DEBUG << "replyStr" << replyStr;
    //    DEBUG << "===============================================================================================================>";

    parseReply(replyStr);

    if(!parser->getXMLHandler()->resDownloadingNow() && reply->error() != QNetworkReply::AuthenticationRequiredError)
    {
        show();
    }

    QApplication::restoreOverrideCursor();
}

bool WebContentManager::parseReply(const QString &str)
{
    QXmlSimpleReader reader;
    QXmlInputSource source;

    parser.reset(new XMLParser);

    source.setData(str.toAscii());

    reader.setContentHandler(parser.data());
    reader.setErrorHandler(parser.data());

    return reader.parse(&source);
}

void WebContentManager::show()
{
    ContentManager::show();

    cashIcons();

    if(getRequest().url() != GET_FULL_ROOT_CONTENT)
    {
        isRoot = false;

        treeWidgetItems.push_back(new QTreeWidgetItem(panel));
        treeWidgetItems.last()->setText(0, PARENT_FOLDER_SIGN);
    }
    else
    {
        isRoot = true;
    }

    for(int i = 0; i < normalizedItems.count(); ++i)
    {
        addItem(normalizedItems[i]);
    }

    if(SettingsManager().initialLoading()) emit signalFirstPanelIsLoaded();

    QString url(getRequest().url().toString());

    if(!pathesURLs.contains(url)) pathesURLs.push_back(url);

    connect(panel->header(),SIGNAL(sectionClicked(int)), this, SLOT(slotSectionClicked(int)));
}

void WebContentManager::setItems(Items::Data::ESortOrder itemSortOrder, Qt::SortOrder sortOrder)
{
    normalizedItems = parser->getXMLHandler()->getItemInfo()->getItems();

    if(!normalizedItems.isEmpty()) parentData = normalizedItems.takeFirst();

    parser->getXMLHandler()->getItemInfo()->sort(normalizedItems, itemSortOrder, sortOrder);
}

void WebContentManager::addItem(const Items::Data &itemData)
{
    FileIconProvider fileIconProvider;
    QIcon icon;

    if(itemData.type == FOLDER_TYPE_STR) icon = fileIconProvider.icon(QFileIconProvider::Folder);
    else icon = fileIconProvider.getIconByExt(itemData.name);

    treeWidgetItems.push_back(new QTreeWidgetItem(panel));

    treeWidgetItems.last()->setText(0, itemData.name);
    treeWidgetItems.last()->setIcon(0, icon);
    treeWidgetItems.last()->setText(1, itemData.owner);
    treeWidgetItems.last()->setText(2, getDate(itemData.dateTime));
    treeWidgetItems.last()->setText(3, getSize(itemData.fileSize));
}

void WebContentManager::cashIcons()
{
    FileIconProvider fileIconProvider;

    for(int i = 0; i < normalizedItems.count(); ++i)
    {
        fileIconProvider.setExtProviderFile(normalizedItems[i].name);
    }
}

QString WebContentManager::getParentFolder() const
{
    return pathesURLs.last();
}

Items::Data WebContentManager::getParentFolderInfo() const
{
    return parentData;
}

QString WebContentManager::back()
{
    QString prevLink;

    if(!pathesURLs.isEmpty())
    {
        pathesURLs.pop_back();
        prevLink = pathesURLs.last();
        pathesURLs.pop_back();
    }

    return prevLink;
}

QStringList WebContentManager::getPathesURLs() const
{
    return pathesURLs;
}

void WebContentManager::setPathesURLs(const QStringList &pathesURLsStrList)
{
    pathesURLs = pathesURLsStrList;
}

Items::Data WebContentManager::getCurrentItem()
{
    int index;

    if(isRoot) index = panel->currentIndex().row();
    else index = panel->currentIndex().row() - 1;

    if(index < 0) index = 0;

    return  normalizedItems[index];
}

void WebContentManager::getItemsDataByIndexes(QList<int> &indexes, QList<Items::Data> &folders, QList<Items::Data> &files)
{      
    for(int i = 0; i < indexes.count(); ++i)
    {
        if(normalizedItems[indexes[i]].type == FOLDER_TYPE_STR) folders.push_back(normalizedItems[indexes[i]]);
        if(normalizedItems[indexes[i]].type == FILE_TYPE_STR) files.push_back(normalizedItems[indexes[i]]);
    }
}

int WebContentManager::getIndexByItemData(QTreeWidget *treeWidget, Items::Data &itemData) const
{
    int index = -1;
    int shiftItemsValue = 0;

    if(treeWidget->topLevelItemCount() > 0)
    {
        FilePanel *filePanel = SDriveEngine::inst()->getFilePanel(SettingsManager().currentPanel());
        if(filePanel->hasItemParentSign(treeWidget->topLevelItem(0))) shiftItemsValue = 1;
    }

    for(int i = 0; i < normalizedItems.count(); ++i)
    {
        if(normalizedItems[i] == itemData)
        {
            index = i;
            break;
        }
    }

    return index + shiftItemsValue;
}

void WebContentManager::slotSectionClicked(int logicalIndex)
{
    Q_UNUSED(logicalIndex);
    sectionClicked();
}

void WebContentManager::updateItemsState()
{
    Items::Data::ESortOrder itemSortOrder;
    Qt::SortOrder sortOrder;

    QByteArray values(SettingsManager().restorePanelHeaderState(panelNum));

    if(values.isEmpty())
    {
        itemSortOrder = Items::Data::ETypeName;
        sortOrder = Qt::AscendingOrder;

        panel->header()->setSortIndicator(0, sortOrder);
    }
    else
    {
        panel->header()->restoreState(values);

        itemSortOrder = static_cast<Items::Data::ESortOrder> (panel->header()->sortIndicatorSection());
        sortOrder =  static_cast<Qt::SortOrder> (panel->header()->sortIndicatorOrder());
    }

    setItems(itemSortOrder, sortOrder);
}




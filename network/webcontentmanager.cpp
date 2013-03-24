#include <QHeaderView>
#include <QXmlSimpleReader>
#include "webcontentmanager.h"
#include "share/commontools.h"
#include "settings/settingsmanager.h"
#include "core/driveengine.h"
#include "gui/tools/fileiconprovider.h"
#include "share/debug.h"

WebContentManager::WebContentManager(FilePanel *fp, QObject *parent) :
    ContentManager(fp)
{
    parser.reset(new XMLParser);
    pathes = SettingsManager().pathesURLs(panelNum);
}

WebContentManager::~WebContentManager()
{
    pathes.clear();
}

void WebContentManager::get(const QString &resourcePointer)
{    
    QApplication::setOverrideCursor(Qt::WaitCursor);

    setCurrentPanelState(resourcePointer);

    CommonTools::setHeader(SettingsManager().accessToken(), request);
    getRequest(resourcePointer);
}

void WebContentManager::slotReplyFinished(QNetworkReply* reply)
{
    //CommonTools::logToFile(QString("ParserReply ") + ".txt", replyStr.toAscii());

    //DEBUG << "<===============================================================================================================";
    //DEBUG << "replyStr" << replyStr;
    //DEBUG << "===============================================================================================================>";

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
    cashIcons();

    ContentManager::show();

    addPath(getLastRequest().url().toString());
    SettingsManager().setPathesURLs(panelNum, pathes);

    for(int i = 0; i < normalizedItems.count(); ++i)
    {
        addItem(normalizedItems[i]);
    }

    if(SettingsManager().initialLoading()) emit signalFirstPanelIsLoaded();
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
    QDateTime fileDateTime = QDateTime::fromString(itemData.dateTime, Qt::ISODate);
    qint64 size = itemData.fileSize.toLongLong();

    if(itemData.type == FOLDER_TYPE_STR) icon = fileIconProvider.icon(QFileIconProvider::Folder);
    else icon = fileIconProvider.getIconByExt(itemData.name);

    treeWidgetItems.push_back(new QTreeWidgetItem(panel));

    treeWidgetItems.last()->setText(0, itemData.name);
    treeWidgetItems.last()->setIcon(0, icon);
    treeWidgetItems.last()->setText(1, itemData.owner);
    treeWidgetItems.last()->setText(2, getDate(fileDateTime));
    treeWidgetItems.last()->setText(3, getSize(size));
}

void WebContentManager::cashIcons()
{
    FileIconProvider fileIconProvider;

    for(int i = 0; i < normalizedItems.count(); ++i)
    {
        fileIconProvider.setExtProviderFile(normalizedItems[i].name);
    }
}

Items::Data WebContentManager::getParentFolderInfo() const
{
    return parentData;
}

Items::Data WebContentManager::getCurrentItem()
{
    int index;

    if(isRoot()) index = panel->currentIndex().row();
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
        if(hasItemParentSign(treeWidget->topLevelItem(0))) shiftItemsValue = 1;
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

void WebContentManager::updateItemsState(QByteArray &values)
{
    Items::Data::ESortOrder itemSortOrder;
    Qt::SortOrder sortOrder;

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

void WebContentManager::update()
{
    SettingsManager settingsManager;
    QString disc;
    QString currentAccountName(settingsManager.currentAccount(panelNum));

    settingsManager.setCurrentPanel(panelNum);

    disc = settingsManager.accountDrive(currentAccountName);
    disc += QString(":");
    disc += QDir::toNativeSeparators("/");

    pathLabel->setText(disc + settingsManager.currentFolderPath(panelNum));
    drivesComboBox->setToolTip(tr("Email: ") + currentAccountName + tr("\nName: ") + settingsManager.name(currentAccountName));

    get(settingsManager.currentFolderUrl(panelNum));

    ComboBoxItem drivesMap;
    QIcon icon(QApplication::style()->standardIcon(QStyle::SP_DriveFDIcon));
    QStringList accounts(settingsManager.accounts());

    foreach(QString account, accounts)
    {
        QString driveLetter (settingsManager.accountDrive(account));
        QMap<QString, QIcon> drivesAdditionalInfo;

        drivesAdditionalInfo[account] = icon;
        drivesMap[driveLetter] = drivesAdditionalInfo;
    }

    fillComboBox(drivesMap, currentAccountName);
}

bool WebContentManager::isRoot()
{
    return getLastRequest().url() == GET_FULL_ROOT_CONTENT;
}

void WebContentManager::accountsComboBoxItemActivated(const QString &text)
{
    int beginPos = text.indexOf(ACCOUNT_SEPARATOR_BEGIN) + ACCOUNT_SEPARATOR_BEGIN.length();
    int length = text.lastIndexOf(ACCOUNT_SEPARATOR_END) - beginPos;

    QString accountName(text.mid(beginPos, length));
    SettingsManager settingsManager;

    if(settingsManager.currentAccount(panelNum) != accountName)
    {
        settingsManager.setCurrentAccount(panelNum, accountName);
        update();
    }
}

void WebContentManager::showFilesOnPanel(QTreeWidgetItem *item)
{
    const QString itemName(item->data(0, Qt::DisplayRole).toString());

    if(hasItemParentSign(item))
    {
        performShowFiles(parentFolder(), itemName, EBackward);
    }
    else
    {
        if(isDir())
        {
            QString query(GET_FILES_IN_FOLDER);

            query += CommonTools::getIDFromURL(getCurrentItem().self);
            query += (CONTENTS + MAX_RESULTS);

            performShowFiles(query, itemName, EForward);
        }
    }
}

void WebContentManager::setCurrentPanelState(const QString &url)
{
    DEBUG;
    SettingsManager settingsManager;

    settingsManager.setCurrentFolderURL(panelNum, url);

    QString fullPath(pathLabel->text());
    int beginPos = fullPath.indexOf(QDir::toNativeSeparators("/")) + 1;
    int length = fullPath.length() - beginPos;

    settingsManager.setCurrentFolderPath(panelNum, fullPath.mid(beginPos, length));
    settingsManager.setPathesURLs(panelNum, pathes);
}

void WebContentManager::performShowFiles(const QString &query, const QString &name, EPath path)
{
    setPanelDisplayingPath(name, path);
    get(query);
}

void WebContentManager::setPanelDisplayingPath(const QString &name, EPath path)
{
    QString pathDividerSign(QDir::toNativeSeparators("/"));
    int labelTextlength = pathLabel->text().length();
    int discStrLength = getDiscLength();

    switch(path)
    {
    case EForward:
    {
        QString divider((labelTextlength == discStrLength) ? "" : pathDividerSign);
        pathLabel->setText(pathLabel->text() += (divider + name));
    }
        break;
    case EBackward:
    {
        int pos = pathLabel->text().lastIndexOf(pathDividerSign);
        int removeCount = labelTextlength - (pos + 1) + 1;

        if(pos == (discStrLength - 1)) ++pos;

        pathLabel->setText(pathLabel->text().remove(pos, removeCount));
    }
        break;
    }
}

bool WebContentManager::isDir()
{
    return (getCurrentItem().type == FOLDER_TYPE_STR);
}

int WebContentManager::getDiscLength() const
{
    SettingsManager settingsManager;
    QString drive (settingsManager.accountDrive(settingsManager.currentAccount(panelNum)));

    drive += QString(":");
    drive += QDir::toNativeSeparators("/");

    return drive.length();
}

QString WebContentManager::parentFolder()
{
    QString parentFolderUrl (GET_FULL_ROOT_CONTENT);

    if(!pathes.isEmpty()) pathes.pop_back();
    if(!pathes.isEmpty()) parentFolderUrl = pathes.last();

    return parentFolderUrl;
}




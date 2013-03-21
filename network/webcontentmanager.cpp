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
}

WebContentManager::~WebContentManager()
{
    pathesURLs.clear();
}

void WebContentManager::get(const QString &resourcePointer)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    CommonTools::setHeader(SettingsManager().accessToken(), request);

    getRequest(resourcePointer);
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
    cashIcons();

    ContentManager::show();

//    if(getRequest().url() != GET_FULL_ROOT_CONTENT)
//    {
//        isRoot = false;

//        treeWidgetItems.push_back(new QTreeWidgetItem(panel));
//        treeWidgetItems.last()->setText(0, PARENT_FOLDER_SIGN);
//    }
//    else
//    {
//        isRoot = true;
//    }

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

void WebContentManager::update()
{
    SettingsManager settingsManager;
    QString disc;
    QString accountName(settingsManager.currentAccount(panelNum));

    settingsManager.setCurrentPanel(panelNum);

    disc = settingsManager.accountDisc(accountName);
    disc += QString(":");
    disc += QDir::toNativeSeparators("/");

    pathLabel->setText(disc + settingsManager.currentFolderPath(panelNum));
    drivesComboBox->setToolTip(tr("Email: ") + accountName + tr("\nName: ") + settingsManager.name(accountName));

    setPathesURLs(settingsManager.pathesURLs(panelNum));
    get(settingsManager.currentFolderURL(panelNum));

    fillComboBox(settingsManager.accountsWithLetters(), accountName);
}

void WebContentManager::fillComboBox(QMap<QString, QString> accountsMap, const QString &currentAccount)
{
    QStringList keys(accountsMap.keys());

    drivesComboBox->clear();

    for(int i = 0; i < keys.count(); ++i)
    {
        QString discLetter(keys[i]);

        discLetter = discLetter.rightJustified(2,' ');
        discLetter = discLetter.leftJustified(6, ' ');

        drivesComboBox->addItem(discLetter + ACCOUNT_SEPARATOR_BEGIN + accountsMap[keys[i]] + ACCOUNT_SEPARATOR_END);
        drivesComboBox->setItemIcon(i, QIcon(QApplication::style()->standardIcon(QStyle::SP_DriveFDIcon)));

        if(currentAccount == accountsMap[keys[i]] && drivesComboBox->currentIndex() != i)
        {
            drivesComboBox->setCurrentIndex(i);
        }
    }

    drivesComboBox->setMinimumWidth(80);
}

bool WebContentManager::isRootFolder()
{
    return getRequest().url() == GET_FULL_ROOT_CONTENT;
}

void WebContentManager::accountsComboBoxItemActivated(const QString &text)
{
    DEBUG;

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
        performShowFiles(back(), itemName, EBackward);
    }
    else
    {
        if(isFolder())
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
    SettingsManager settingsManager;

    settingsManager.setCurrentFolderURL(panelNum, url);

    QString fullPath(pathLabel->text());
    int beginPos = fullPath.indexOf(QDir::toNativeSeparators("/")) + 1;
    int length = fullPath.length() - beginPos;

    settingsManager.setCurrentFolderPath(panelNum, fullPath.mid(beginPos, length));
    settingsManager.setPathesURLs(panelNum, getPathesURLs());
}

void WebContentManager::performShowFiles(const QString &query, const QString &name, EPath path)
{
    setPanelDisplayingPath(name, path);
    get(query);
    setCurrentPanelState(query);
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

bool WebContentManager::isFolder(void)
{
    return (getCurrentItem().type == FOLDER_TYPE_STR);
}

int WebContentManager::getDiscLength() const
{
    SettingsManager settingsManager;
    QString disc(settingsManager.accountDisc(settingsManager.currentAccount(panelNum)));

    disc += QString(":");
    disc += QDir::toNativeSeparators("/");

    return disc.length();
}




#ifndef WEBCONTENTMANAGER_H
#define WEBCONTENTMANAGER_H

#include <QLabel>
#include "contentmanager.h"
#include "parsers/xmlparser.h"
#include "network/networkmanager.h"
#include "gui/controls/combobox.h"

class WebContentManager : public NetworkManager, public ContentManager
{
    Q_OBJECT
public:
    enum EPath { EForward, EBackward };

    explicit WebContentManager(FilePanel *fp, QObject *parent = 0);
    ~WebContentManager();

signals:
    void signalFirstPanelIsLoaded();

private slots:
    void slotReplyFinished(QNetworkReply* reply);
    void slotSectionClicked(int logicalIndex);

public:
    void get(const QString &resourcePointer);
    QString getParentFolder() const;
    void update();
    void accountsComboBoxItemActivated(const QString &text);
    void showFilesOnPanel(QTreeWidgetItem *item);

protected:
    QString back();
    void updateItemsState();
    void show();
    void fillAcountsComboBox(QMap<QString, QString> accountsMap, const QString &currentAccount);

public:   
    Items::Data getParentFolderInfo() const;
    Items::Data getCurrentItem();
    QStringList getPathesURLs() const;
    void setPathesURLs(const QStringList &pathesURLsStrList);
    void getItemsDataByIndexes(QList<int> &indexes, QList<Items::Data> &folders, QList<Items::Data> &files);
    int getIndexByItemData(QTreeWidget *treeWidget, Items::Data &itemData) const;
    void cashIcons();
    void setCurrentPanelState(const QString &url);

private:
    bool parseReply(const QString &str);
    void setItems(Items::Data::ESortOrder itemSortOrder, Qt::SortOrder sortOrder);
    void addItem(const Items::Data &itemData);
    void performShowFiles(const QString &query, const QString &name, EPath path);
    void setPanelDisplayingPath(const QString &name, EPath path);
    bool isFolder();
    int getDiscLength() const;

private:
    QScopedPointer<XMLParser> parser;
    QStringList pathesURLs;
    Items::Data parentData;
    QList<Items::Data> normalizedItems;
};

#endif // WEBCONTENTMANAGER_H

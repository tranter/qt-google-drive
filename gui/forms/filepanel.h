#ifndef FILEPANEL_H
#define FILEPANEL_H

#include <QWidget>
#include <QTreeWidget>
#include <QLabel>
#include "gui/controls/toolbar.h"
#include "gui/controls/combobox.h"

namespace Ui {
class FilePanel;
}

class FilePanel : public QWidget
{
    Q_OBJECT

public:
    explicit FilePanel(QWidget *parent = 0);
    ~FilePanel();

public:
    QTreeWidget* getFileView(void) const;
    QLabel* getPathLabel(void) const;
    ToolBar* getAccountsToolBar(void) const;
    ComboBox* getAccountsComboBox(void) const;
    void fillComboBox(QMap<QString, QString> accountsMap, int index);

private:
    void init(void);

private:
    Ui::FilePanel *ui;
    ToolBar *accountsToolBar;
    ComboBox *accountsComboBox;
};

#endif // FILEPANEL_H

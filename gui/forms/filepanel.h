#ifndef FILEPANEL_H
#define FILEPANEL_H

#include <QWidget>
#include <QTreeWidget>
#include <QLabel>

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
    QLabel *getPathLabel(void) const;

private:
    Ui::FilePanel *ui;
};

#endif // FILEPANEL_H

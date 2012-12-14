#ifndef FILEPANEL_H
#define FILEPANEL_H

#include <QWidget>

namespace Ui {
class FilePanel;
}

class FilePanel : public QWidget
{
    Q_OBJECT
    
public:
    explicit FilePanel(QWidget *parent = 0);
    ~FilePanel();
    
private:
    Ui::FilePanel *ui;
};

#endif // FILEPANEL_H

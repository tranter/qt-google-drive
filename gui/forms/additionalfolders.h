#ifndef ADDITIONALFOLDERS_H
#define ADDITIONALFOLDERS_H

#include <QWidget>

namespace Ui {
class AdditionalFolders;
}

class AdditionalFolders : public QWidget
{
    Q_OBJECT
    
public:
    explicit AdditionalFolders(QWidget *parent = 0);
    ~AdditionalFolders();
    
private:
    Ui::AdditionalFolders *ui;
};

#endif // ADDITIONALFOLDERS_H

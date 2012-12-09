#ifndef DRIVETREE_H
#define DRIVETREE_H

#include <QWidget>

namespace Ui {
class DriveTree;
}

class DriveTree : public QWidget
{
    Q_OBJECT
    
public:
    explicit DriveTree(QWidget *parent = 0);
    ~DriveTree();
    
private:
    Ui::DriveTree *ui;
};

#endif // DRIVETREE_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "driveengine.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    void init(void);

signals:
    void testClick(void);
    
private slots:
    void on_testButton_clicked();

private:
    Ui::MainWindow *ui;
    DriveEngine* driveEngine;
};

#endif // MAINWINDOW_H

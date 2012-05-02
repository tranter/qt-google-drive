#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "driveengine.h"

namespace Ui {
class MainWindow;
}

class DriveEngine;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    void init(void);
    Ui::MainWindow* getUi(void);

//public:
//    static Ui::MainWindow* ui;

signals:
    void getButtonClick(void);
    void postButtonClick(void);
    
private slots:
    void on_getButton_clicked();

    void on_postButton_clicked();

private:
    Ui::MainWindow* ui;

    DriveEngine* driveEngine;
};

class UiInstance
{
public:
    static Ui::MainWindow* Instance();

    static Ui::MainWindow* ui;
private:
        UiInstance(){};
        UiInstance(UiInstance& root){};
        UiInstance& operator=(UiInstance&){};
};

#endif // MAINWINDOW_H

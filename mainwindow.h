#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "driveengine.h"
#include <QDateTime>

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

private slots:
    void slotloginDone();
    void slotUpdateModel();

signals:
    void siganalGet(void);
    void signalPost(void);
    
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

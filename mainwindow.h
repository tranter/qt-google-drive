#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "driveengine.h"
#include <QDateTime>
#include <QTextCodec>

namespace Ui
{
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
    virtual bool eventFilter(QObject *object, QEvent *event);

public:
    void init(void);

private:
    void setConnections(void);
    bool CheckReg(void);

private slots:
    void slotloginDone();
    void slotUpdateFileList();
    
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

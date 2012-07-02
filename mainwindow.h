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

signals:
    void signalDel(QObject* object);
    
private:
    DriveEngine* driveEngine;
};

class UiInstance
{
public:
    static Ui::MainWindow* Instance();

private:
    UiInstance(){};
    Q_DISABLE_COPY(UiInstance);

private:
    static Ui::MainWindow* ui;
};

class DriveEngineInstance
{
public:
    static DriveEngine* Instance();

private:
    DriveEngineInstance(){};
    Q_DISABLE_COPY(DriveEngineInstance);

private:
    static DriveEngine* driveEngine;
};

#endif // MAINWINDOW_H

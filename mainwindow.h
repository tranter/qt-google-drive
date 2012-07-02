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

signals:
    void signalDel(QObject* object);
};

class SUi
{
public:
    static Ui::MainWindow* Inst();
    inline static void FreeInst(void){if(ui) delete ui; ui = NULL;}

private:
    SUi(){}
    ~SUi();
    Q_DISABLE_COPY(SUi);

private:
    static Ui::MainWindow* ui;
};

class SDriveEngine
{
public:
    static DriveEngine* Inst();
    inline static void FreeInst(void){if(driveEngine) delete driveEngine; driveEngine = NULL;}

private:
    SDriveEngine(){}
    ~SDriveEngine();
    Q_DISABLE_COPY(SDriveEngine);

private:
    static DriveEngine* driveEngine;
};

#endif // MAINWINDOW_H

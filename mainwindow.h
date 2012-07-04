#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "driveengine.h"
#include <QDateTime>
#include <QTextCodec>
#include "singleton.h"

namespace Ui
{
 class MainWindow;
}

class DriveEngine;
typedef TSingleton<DriveEngine> TDriveEngine;

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
    static Ui::MainWindow* Inst(void);
    static void FreeInst(void);

private:
    SUi();
    ~SUi();
    Q_DISABLE_COPY(SUi)

private:
    static Ui::MainWindow* ui;
};

class SDriveEngine
{
public:
    static DriveEngine* Inst(QObject *parent = 0);
    static void FreeInst(void);

private:
    SDriveEngine();
    ~SDriveEngine();
    Q_DISABLE_COPY(SDriveEngine)

private:
    static DriveEngine* driveEngine;
};

#endif // MAINWINDOW_H

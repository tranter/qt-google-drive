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

typedef TSingleton<Ui::MainWindow> SUi;
typedef TSingleton<DriveEngine> SDriveEngine;

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
    bool checkReg(void);

private slots:
    void slotloginDone();

signals:
    void signalDel(QObject* object);
};

#endif // MAINWINDOW_H

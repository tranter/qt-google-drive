#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void init(void);

private:
    virtual bool eventFilter(QObject *object, QEvent *event);

private:
    void setConnections(void);

private slots:
    void slotloginDone();
};

#endif // MAINWINDOW_H

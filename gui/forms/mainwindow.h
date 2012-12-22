#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "share/singleton.h"
#include "network/queries.h"

namespace Ui
{
 class MainWindow;
}

class DriveEngine;

typedef TSingleton<Ui::MainWindow> SUi;

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
    void slotLogged(void);
    void slotAccessTokenRequired(void);
    void slotAccountInfoReadyToUse(void);
    void slotTestAuth(void);
};

#endif // MAINWINDOW_H

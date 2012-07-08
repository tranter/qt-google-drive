#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <QObject>
#include <QKeyEvent>

class OperationsUI;

typedef void (OperationsUI::*KeyActionFunc)(QObject*);

class EventHandler : public QObject
{
    Q_OBJECT
public:
    explicit EventHandler(OperationsUI* oUI, QObject *parent = 0);
    ~EventHandler();

public:
    bool event(QObject *object, QEvent *event);
    void setKeyAction(Qt::Key key, KeyActionFunc keyActionFunc);

private:
    bool keyPressEvent(QObject *object, QEvent *event);

private:
    QMap<Qt::Key, KeyActionFunc> keyActions;
    OperationsUI* operationsUI;
};

#endif // EVENTHANDLER_H

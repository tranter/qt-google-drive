#include "eventhandler.h"
#include "mainwindow.h"
#include "operationsui.h"
#include <QDebug>

EventHandler::EventHandler(OperationsUI *oUI, QObject *parent) :
    operationsUI(oUI),
    QObject(parent)
{
}

EventHandler::~EventHandler()
{
  keyActions.clear();
}

bool EventHandler::event(QObject *object, QEvent *event)
{
    bool ret = false;

    if (event->type() == QEvent::KeyPress) ret = keyPressEvent(object, event);

    return ret;
}

bool EventHandler::keyPressEvent(QObject *object, QEvent *event)
{
    QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
    Qt::Key key = static_cast<Qt::Key> (keyEvent->key());

    if(keyActions.contains(key))
    {
        (operationsUI->*keyActions[key])(object);
        return true;
    }

    return false;
}

void EventHandler::setKeyAction(Qt::Key key, KeyActionFunc keyActionFunc)
{
 keyActions[key] = keyActionFunc;
}

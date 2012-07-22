#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <QKeyEvent>
#include <QDebug>

template <typename T>
class EventHandler
{
public:
    typedef void (T::*KeyActionFunc)(QObject*);

    explicit EventHandler(T* hUI) : handlerUI(hUI){}
    ~EventHandler() {keyActions.clear();}

public:
    bool event(QObject *object, QEvent *event);
    void setKeyAction(Qt::Key key, KeyActionFunc keyActionFunc);

private:
    bool keyPressEvent(QObject *object, QEvent *event);

private:
    QMap<Qt::Key, KeyActionFunc> keyActions;
    T* handlerUI;
};

template <typename T>
bool EventHandler<T>::event(QObject *object, QEvent *event)
{
    bool ret = false;
    if (event->type() == QEvent::KeyPress) ret = keyPressEvent(object, event);
    if (event->type() == QEvent::GrabMouse) qDebug() << "---->" << object->objectName();
    return ret;
}

template <typename T>
bool EventHandler<T>::keyPressEvent(QObject *object, QEvent *event)
{
    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
    Qt::Key key = static_cast<Qt::Key> (keyEvent->key());

    if(keyActions.contains(key))
    {
        (handlerUI->*keyActions[key])(object);
        return true;
    }

    return false;
}

template <typename T>
void EventHandler<T>::setKeyAction(Qt::Key key, KeyActionFunc keyActionFunc)
{
 keyActions[key] = keyActionFunc;
}

#endif // EVENTHANDLER_H

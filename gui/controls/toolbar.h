#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>
#include "tools/painterhelper.h"

class ToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit ToolBar(QWidget *parent = 0, const QString &styleSheet = "", int height);
    
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // TOOLBAR_H

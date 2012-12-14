#include "toolbar.h"
#include <QLayout>
#include <QStyleOption>

ToolBar::ToolBar(QWidget *parent, const QString &styleSheet, int height) :
    QToolBar(parent)
{ 
    setStyleSheet(styleSheet);
    setFixedHeight(height);

    const int offset = 0;

    layout()->setContentsMargins(offset, offset, offset, offset);
}

void ToolBar::paintEvent(QPaintEvent *event)
{    
    Q_UNUSED(event);

    QStyleOption opt;
    QPainter p(this);

    opt.init(this);

    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

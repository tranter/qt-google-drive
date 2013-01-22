#include "spacer.h"

Spacer::Spacer(QWidget *parent, const QString &styleSheet, bool frame) :
    QFrame(parent),
    isFrame(frame)
{
    setVisible(true);
    setStyleSheet(styleSheet);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

Spacer::Spacer(QWidget *parent, int width, int height, const QString &styleSheet, bool frame) :
    QFrame(parent),
    isFrame(frame)
{
    setVisible(true);
    setStyleSheet(styleSheet);

    setFixedWidth(width);
    setFixedHeight(height);
}

void Spacer::paintEvent(QPaintEvent *event)
{
    if(!isFrame) return;

    Q_UNUSED(event)

#ifndef Q_OS_MAC
    const int colorValue = 61;

    PainterHelper painterHelper(this);
    painterHelper.drawFrame(rect(), QColor(colorValue, colorValue, colorValue));
#endif
}

#include "painterhelper.h"

PainterHelper::PainterHelper(QPaintDevice *device) :
    QPainter(device)
{
    pen.setColor(QColor(61, 61, 61));
}

void PainterHelper::drawFrame(QRect rect, QColor)
{    
    QRect paintRect(rect);

    setPen(pen);

    const int offset = 1;

    paintRect.setY(offset);
    paintRect.setHeight(rect.height() - offset * 2);

    drawRect(paintRect);
}

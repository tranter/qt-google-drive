#ifndef PAINTERHELPER_H
#define PAINTERHELPER_H

#include <QPainter>
#include <QPen>

class PainterHelper : public QPainter
{
public:
    PainterHelper(QPaintDevice *device);

public:
    void drawFrame(QRect rect, QColor);

private:
     QPen pen;
};

#endif // PAINTERHELPER_H

#include "separator.h"
#include <QPainter>
#include <QStyleOption>

Separator::Separator(int width, int height, QWidget *parent)
    : QWidget(parent)
{
    setFixedWidth(width);
    setFixedHeight(height);
}

void Separator::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    QStyleOption option;

    option.rect = rect();
    option.state = QStyle::State_Horizontal;
    option.palette = palette();

    style()->drawPrimitive(QStyle::PE_IndicatorToolBarSeparator, &option, &painter, this);
}

#include "toolbutton.h"
#include <QApplication>
#include <QStyle>

ToolButton::ToolButton()
{
    init();
}

void ToolButton::init()
{
    setFixedWidth(25);
    setCursor(Qt::PointingHandCursor);
    setIcon(QIcon(QApplication::style()->standardIcon(QStyle::SP_ComputerIcon)));
    setCheckable(true);
}

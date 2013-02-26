#include "spacer.h"

Spacer::Spacer(QWidget *parent, const QString &styleSheet) :
    QFrame(parent)
{
    setVisible(true);
    setStyleSheet(styleSheet);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

Spacer::Spacer(QWidget *parent, int width, int height, const QString &styleSheet) :
    QFrame(parent)
{
    setVisible(true);
    setStyleSheet(styleSheet);

    setFixedWidth(width);
    setFixedHeight(height);
}

void Spacer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

//#ifndef Q_OS_MAC
//    PainterHelper painterHelper(this);
//    painterHelper.drawFrame(rect(), QColor(61, 61, 61));
//#endif
}

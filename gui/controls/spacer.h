#ifndef SPACER_H
#define SPACER_H

#include "../tools/painterhelper.h"
#include <QFrame>

class Spacer : public QFrame
{
    Q_OBJECT
public:
    explicit Spacer(QWidget *parent = 0, const QString &styleSheet = QString(), bool frame = false);
    explicit Spacer(QWidget *parent, int width, int height, const QString &styleSheet = QString(), bool frame = false);

protected:
    void paintEvent(QPaintEvent *event);

private:
    PainterHelper *painterHelper;
};

#endif // SPACER_H


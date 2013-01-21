#ifndef SEPARATOR_H
#define SEPARATOR_H

#include <QWidget>

class Separator : public QWidget
{
    Q_OBJECT
public:
    Separator(int width, int height, QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);

private:
    bool frame;
};

#endif // SEPARATOR_H

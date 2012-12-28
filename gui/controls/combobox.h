#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QComboBox>

class ComboBox : public QComboBox
{
public:
    ComboBox(int width, int height = 24, QWidget *parent = 0);

protected:
    void hidePopup(void);

public:
    void init(int width, int height);
};

#endif // COMBOBOX_H

#include "combobox.h"
#include "share/debug.h"

ComboBox::ComboBox(int width, int height, QWidget *parent):
    QComboBox(parent)
{
    init(width, height);
}

void ComboBox::hidePopup()
{
    QComboBox::hidePopup();
    QComboBox::clearFocus();
}

void ComboBox::init(int width, int height)
{
    setFixedHeight(height);
    setCursor(Qt::PointingHandCursor);
    setMinimumContentsLength(width);
    //setGeometry(QRect(0, 0 , 480, height));
   // setMinimumWidth(180);
    setFrame(false);
    setStyleSheet(QString("%1%2%3%4%5%6%7%8%9")
                  .arg("QComboBox {background-color: qlineargradient(spread:pad, x1:1, y1:0.432, x2:1, y2:0, stop:0 rgba(72, 72, 72, 255), stop:1 rgba(154, 154, 154, 255));}")
                  .arg("QComboBox {font-size: 11px;color: white;min-width: 60px;}")
                  .arg("QComboBox {padding-left: 16px;padding-right: 0px;spacing: 2px;}")
                  .arg("QComboBox::drop-down {subcontrol-origin: padding;subcontrol-position: top right;width: 12px;border-left-style: solid;border-left-color: black;border-left-width: 0px;}")
                  .arg("QComboBox::down-arrow {subcontrol-origin: content;subcontrol-position: center;position: relative;right: 3px;}")
                  .arg("QComboBox::down-arrow:on {position: relative;top: 1px;}")
                  .arg("QComboBox {padding-right: 10px;}")
                  .arg("QComboBox::disabled {color: gray;}")
                  .arg("QComboBox::menu-indicator, QComboBox::down-arrow {image: url(:ico/downArrow);width: 7px;height: 7px;}"));
}

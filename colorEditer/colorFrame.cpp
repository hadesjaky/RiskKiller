#include "colorFrame.h"
#include <QMouseEvent>
#include <QColorDialog>
#include <QPalette>

ColorFrame::ColorFrame(QWidget *parent) :
    QFrame(parent)
{
    setAutoFillBackground(true);
    setFrameShadow(Sunken);
    setFrameShape(WinPanel);

}
void ColorFrame::mousePressEvent(QMouseEvent *e)
{
    QColor color = palette().color(QPalette::Background);
    if(Qt::LeftButton == e->button())
    {
        emit SIG_ColorSelected(color);
    }
    return QFrame::mousePressEvent(e);
}

void ColorFrame::setColor(const QColor &color)
{
    QPalette p;
    p.setColor(QPalette::Background, color);
    setPalette(p);
}

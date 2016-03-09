#include "colorPlan.h"
#include <QDebug>
#include <QWidget>

ColorPlan::ColorPlan(QWidget *parent) : QWidget(parent)
{
    //setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);
    //setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
    //setWindowFlags(Qt::WindowMinimizeButtonHint);
    color = Qt::green;
    setFixedSize(250,65);
    QColor colors[] = {
        Qt::white,
        Qt::black,
        Qt::red,
        Qt::darkRed,
        Qt::green,
        Qt::darkGreen,
        Qt::blue,
        Qt::darkBlue,
        Qt::cyan,
        Qt::darkCyan,
        Qt::magenta,
        Qt::darkMagenta,
        Qt::yellow,
        Qt::darkYellow,
        Qt::gray,
        Qt::darkGray
    };
    for(int i = 0; i <16; i++)
    {
        frames[i] = new ColorFrame(this);
        frames[i]->setColor(colors[i]);
        frames[i]->setGeometry(5 + 30 * (i % 8), 5 + 30 * (i / 8), 25, 25);
        connect(frames[i],SIGNAL(SIG_ColorSelected(QColor)),this,SLOT(selectedColorSLOT(QColor)));
    }

    adjustSize();
}

void ColorPlan::selectedColorSLOT(QColor &c)
{
    qDebug() << 11111;
    color = c;
}


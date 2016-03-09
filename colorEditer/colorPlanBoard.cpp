#include "colorPlanBoard.h"
#include <QDebug>
#include <QRgb>

ColorPlanBoard::ColorPlanBoard(QWidget *parent) : QWidget(parent)
{
    color = Qt::green;
    setFixedSize(250,190);
    QColor colors[] = {
//        Qt::white,
//        Qt::black,
//        Qt::red,
//        Qt::darkRed,
//        Qt::green,
//        Qt::darkGreen,
//        Qt::blue,
//        Qt::darkBlue,
//        Qt::cyan,
//        Qt::darkCyan,
//        Qt::magenta,
//        Qt::darkMagenta,
//        Qt::yellow,
//        Qt::darkYellow,
//        Qt::gray,
//        Qt::darkGray
        QColor(0,0,0),QColor(170,0,0),QColor(0,85,0),QColor(170,85,0),QColor(0,170,0),QColor(170,170,0),QColor(0,255,0),QColor(170,255,0),
        QColor(0,0,127),QColor(170,0,127),QColor(0,85,127),QColor(170,85,127),QColor(0,170,127),QColor(170,170,127),QColor(0,255,127),QColor(170,255,127),
        QColor(0,0,255),QColor(170,0,255),QColor(0,85,255),QColor(170,85,255),QColor(0,170,255),QColor(170,170,255),QColor(0,255,255),QColor(170,255,255),
        QColor(85,0,0),QColor(255,0,0),QColor(85,85,0),QColor(255,85,0),QColor(85,170,0),QColor(255,170,0),QColor(85,255,0),QColor(255,255,0),
        QColor(85,0,127),QColor(255,0,127),QColor(85,85,127),QColor(255,85,127),QColor(85,170,127),QColor(255,170,127),QColor(85,255,127),QColor(255,255,127),
        QColor(85,0,255),QColor(255,0,255),QColor(85,85,255),QColor(255,85,255),QColor(85,170,255),QColor(255,170,255),QColor(85,255,255),QColor(255,255,255)
    };
    for(int i = 0; i <48; i++)
    {
        frames[i] = new ColorFrame(this);
        frames[i]->setColor(colors[i]);
        frames[i]->setGeometry(5 + 30 * (i % 8), 5 + 30 * (i / 8), 25, 25);
        connect(frames[i],SIGNAL(SIG_ColorSelected(QColor)),this,SLOT(selectedColorSLOT(QColor)));
    }
    adjustSize();
    setWindowFlags(Qt::WindowStaysOnTopHint);
}

ColorPlanBoard::~ColorPlanBoard()
{

}
void ColorPlanBoard::selectedColorSLOT(QColor c)
{
    color = c;
    emit SIG_changeColor(color);
}
QColor ColorPlanBoard::getColor()
{
    return color;
}

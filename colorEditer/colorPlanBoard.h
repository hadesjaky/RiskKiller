#ifndef COLORPLANBOARD_H
#define COLORPLANBOARD_H

#include <QWidget>
#include <QFrame>
#include "colorFrame.h"

class ColorPlanBoard : public QWidget
{
    Q_OBJECT
public:
    explicit ColorPlanBoard(QWidget *parent = 0);
    ~ColorPlanBoard();
protected:
    //void mousePressEvent(QMouseEvent *e);
private:
    ColorFrame *frames[48];
    QColor color;
signals:
    void SIG_changeColor(QColor c);//颜色发出去
public slots:
    void selectedColorSLOT(QColor c);
    QColor getColor();
};

#endif // COLORPLANBOARD_H

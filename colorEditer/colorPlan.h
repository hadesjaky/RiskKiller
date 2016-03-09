#ifndef COLORPLAN_H
#define COLORPLAN_H

#include <QWidget>
#include <QEvent>
#include <QMouseEvent>
#include <QFrame>
#include "colorFrame.h"

class ColorPlan : public QWidget
{
    Q_OBJECT

public:
    explicit ColorPlan(QWidget *parent = 0);

protected:
    //void mousePressEvent(QMouseEvent *e);
private:
    ColorFrame *frames[16];
    QColor color;
signals:

public slots:
    void selectedColorSLOT(QColor &c);
};

#endif // COLORPLAN_H

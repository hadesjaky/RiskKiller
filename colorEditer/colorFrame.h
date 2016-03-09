#ifndef COLORFRAME_H
#define COLORFRAME_H

#include <QFrame>
#include <QEvent>

class ColorFrame : public QFrame
{
    Q_OBJECT
public:
    explicit ColorFrame(QWidget *parent = 0);
    void setColor(const QColor &color);
protected:
    void mousePressEvent(QMouseEvent *);
    //void mouseDoubleClickEvent(QMouseEvent *);
signals:
    void SIG_ColorSelected(const QColor &);
public slots:

};

#endif // COLORFRAME_H

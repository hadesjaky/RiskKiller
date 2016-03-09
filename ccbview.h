#ifndef CCBVIEW_H
#define CCBVIEW_H

#include <QMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QList>
#include <QColor>
#include "globaldef.h"
#include "itembase.h"
#include "statMmanager.h"

class CCBView : public QGraphicsView
{
    Q_OBJECT

public:
    CCBView(QWidget *parent = 0);
    void setActivedItem(QGraphicsItem *value);

    qreal getScaling() const;
    void setScaling(const qreal &value);
    void showAll() const;//显示全部item

signals:
    void move();
    void initCheckBox();
    void sendMsg(QString msg);
    void scalingChange(qreal);
    void rotationChange(qreal);
    void showTreeWgt(ItemBase *item);//jaky,点击发送信号，显示在treewidget上

protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
//	void enterEvent(QMouseEvent *e);
//	void leaveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
private:
    ItemBase* activedItem;
    QPointF startPos;
    QPointF endPos;
    QPoint RPos;
    qreal scaling;//视图当前放大倍数
    qreal rotation;
    qreal standardScale;//视图初始化放大倍数
    VIEW_MODE viewMode;
    QRect getRect(const QPointF &beginPoint, const QPointF &endPoint);

    QColor color;//jaky 结点颜色
    void delNode(ItemBase* it);

    void drawFixedPoint(ItemBase* it);
    bool findNeighbourNode(ItemBase* node, ItemBase* fNode, QList<ItemBase*> &pass);
    QList<ItemBase*> repeatOut(QList<ItemBase*> list1, QList<ItemBase*> list2);//去重复
    QList<QList<ItemBase*> > passPath;//保存查询点的路径，多条路径包含多个点

    QMenu *itemMenu;

public slots:
    void drawNetBySomeone(bool b);//结点寻径
    void routePoint(QString str);//定点寻址
    void findNode(QString str);//查找结点
    void setColorNode(QColor c);

    void reflush(int);
    void zoomIn();
    void zoomOut();
    void zoom(qreal proportion=0);
    void rotateClockwise();
    void rotateAntiClockwise();
    void rotateAngle(qreal angle);
    void ItemLayerTop(QAction *act);
};

#endif

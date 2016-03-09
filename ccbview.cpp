#include "ccbview.h"
#include "scene.h"
#include <QtMath>
#include <QScrollBar>
#include <QApplication>
#include <QMessageBox>
#include <QMenu>
#include <QDebug>

#define SSTEP 1.1
#define RSTEP 15.0

CCBView::CCBView(QWidget *parent) :
    QGraphicsView(parent)
{
    setCacheMode(CacheBackground);
    //setDragMode(QGraphicsView::RubberBandDrag);
    setRenderHint(QPainter::Antialiasing);
    //setTransformationAnchor(AnchorUnderMouse);
    //setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    //setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setAlignment(Qt::AlignCenter);

    //setRenderHint(QPainter::Antialiasing, false);
    setDragMode(QGraphicsView::RubberBandDrag);
    //setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    //setSceneRect(-1200,-1200,2400,2400);

    /*QScrollBar* scrollbarH = horizontalScrollBar();
    horizontalScrollBar()->setDisabled(true);

    QScrollBar* scrollbarV = verticalScrollBar();
    verticalScrollBar()->setDisabled(true);
    */
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    //connect(verticalScrollBar(), SIGNAL(sliderMoved(int)), this, SLOT(reflush(int)));
    //connect(horizontalScrollBar(), SIGNAL(sliderMoved(int)), this, SLOT(reflush(int)));
    activedItem = NULL;
    //connect(this, SIGNAL(rubberBandChanged(QRect , QPointF
    //, QPointF  )), this, SLOT(updateSceneRect(const QRectF &)));


    scaling = 1.0;
    standardScale = 1.0;
    rotation = 0.0;

    color = Qt::white;
    itemMenu = NULL;

}


void CCBView::mouseMoveEvent(QMouseEvent *e)
{

    if(QLineF(e->pos(), startPos).length() < QApplication::startDragDistance())
    {
        return;
    }
    QPointF mousePos = mapToScene(e->pos());
    emit sendMsg(tr("鼠标位置: （%1, %2）").arg(mousePos.rx()).arg(mousePos.ry()));
    if(e->buttons() & Qt::LeftButton && viewMode == VIEW)
    {
        QGraphicsView::mouseMoveEvent(e);
        viewport()->update();
    }
    if(e->buttons() & Qt::LeftButton && activedItem != NULL && viewMode == MOVE)
    {
        setCursor(Qt::ClosedHandCursor);
        activedItem->setPos(mousePos);
        emit sendMsg(tr("鼠标位置: （%1, %2）").arg(mousePos.rx()).arg(mousePos.ry()));
        viewport()->update();
    }
    if(StateManager::stateIns()->selectModel)
    {
        QGraphicsView::mouseMoveEvent(e);
        viewport()->update();
    }
}

void CCBView::mousePressEvent(QMouseEvent *e)
{
    //在item上鼠标右键
    if(e->button() == Qt::RightButton && activedItem == itemAt(e->pos()))
    {
        RPos = e->pos();
        QList<QGraphicsItem *> cell = scene()->collidingItems(itemAt(e->pos()));
        if(itemMenu == NULL)
        {
            itemMenu = new QMenu();
            connect(itemMenu,SIGNAL(triggered(QAction*)),this,SLOT(ItemLayerTop(QAction*)));
        }
        else
        {
            delete itemMenu;
            itemMenu = NULL;
            itemMenu = new QMenu();
            connect(itemMenu,SIGNAL(triggered(QAction*)),this,SLOT(ItemLayerTop(QAction*)));
        }
        QAction *a;
        for(int i=0;i<cell.size();++i)
        {
            a = new QAction(dynamic_cast<ItemBase*>(cell[i])->getName(),this);
            itemMenu->addAction(a);
        }
        itemMenu->exec(e->globalPos());
        viewMode = VIEW;
    }
    //鼠标左键动作，分为在item上，和不在item上
    if(e->button() == Qt::LeftButton )
    {
        startPos = e->pos();
        if(QGraphicsItem * t = itemAt(e->pos()))
        {
            if(StateManager::stateIns()->mergeNodeAct && activedItem!=NULL && activedItem != dynamic_cast<ItemBase*>(t))//合并
            {
                ItemBase * newItem = dynamic_cast<ItemBase*>(t);
                //activedItem加到newItem中
                ItemBase::mergeMethod(newItem, activedItem);
                Scene::Instance()->delSceneNode(dynamic_cast<ItemBase*>(t));
                activedItem->hide();
                delNode(dynamic_cast<ItemBase*>(activedItem));
                delete dynamic_cast<ItemBase*>(activedItem);
            }
            emit showTreeWgt(dynamic_cast<ItemBase*>(t));
            setActivedItem(t);
            if(StateManager::stateIns()->hiddenNodeAct)//隐藏
            {
                t->hide();
                return;
            }
            if(StateManager::stateIns()->colorBoard)//调色板
            {
                activedItem->setItemColor(color);
            }
            if(StateManager::stateIns()->delNodeAct)//删除
            {
                delNode(dynamic_cast<ItemBase*>(t));
                delete dynamic_cast<ItemBase*>(t);
            }
            viewMode = MOVE;
            viewport()->update();
        }
        else
        {
            viewMode = VIEW;
            QGraphicsView::mousePressEvent(e);
        }
    }
}

void CCBView::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        if(StateManager::stateIns()->colorBoard)
        {
            endPos = e->pos();
            QRect rect = getRect(startPos, endPos);
            QList<QGraphicsItem *> _listItem = items(rect);
            ItemBase *b;
            for(int i=0; i<_listItem.size();++i)
            {
                b = dynamic_cast<ItemBase*>(_listItem[i]);
                b->setItemColor(color);
            }
        }
        viewMode = VIEW;
        QGraphicsView::mousePressEvent(e);
        setCursor(Qt::ArrowCursor);
        viewport()->update();
    }
}
void CCBView::mouseDoubleClickEvent(QMouseEvent *e)
{
    activedItem->setSelected(false);
    activedItem = NULL;
    QGraphicsView::mouseDoubleClickEvent(e);
}
void CCBView::wheelEvent(QWheelEvent *e)
{
    if (e->delta() > 0)
    {
        zoomIn();
    }
    else
    {
        zoomOut();
    }
    //QGraphicsView::wheelEvent(e);
}
QRect CCBView::getRect(const QPointF &beginPoint, const QPointF &endPoint)
{
    int x,y,width,height;
    width = qAbs(beginPoint.x() - endPoint.x());
    height = qAbs(beginPoint.y() - endPoint.y());
    x = beginPoint.x() < endPoint.x() ? beginPoint.x() : endPoint.x();
    y = beginPoint.y() < endPoint.y() ? beginPoint.y() : endPoint.y();

    return QRect(x,y,width,height);
}
qreal CCBView::getScaling() const
{
    return scaling;
}

void CCBView::setScaling(const qreal &value)
{
    scaling = value;
}
void CCBView::showAll() const
{
    QMap<QString, ItemBase*> allItems = Scene::Instance()->getItemContainer();
    QMap<QString, ItemBase*>::const_iterator iter = allItems.constBegin();
    while(iter != allItems.constEnd())
    {
        iter.value()->show();
        iter++;
    }
    viewport()->update();
}

void CCBView::zoomIn()
{
    scaling *= SSTEP;
    scale(SSTEP, SSTEP);
    QList<QGraphicsItem *> items = this->items();
    QList<QGraphicsItem *>::const_iterator iter = items.constBegin();
    while(iter != items.constEnd())
    {
        (*iter)->setScale(1.0/scaling);
        iter++;
    }
    emit sendMsg(tr("当前视图比例： %1 倍").arg(scaling));
    emit scalingChange(scaling);
}

void CCBView::zoomOut()
{
    if(scaling < 0.01)
        return;
    scaling /= SSTEP;
    scale(1.0/SSTEP, 1.0/SSTEP);

    QList<QGraphicsItem *> items = this->items();
    QList<QGraphicsItem *>::const_iterator iter = items.constBegin();
    while(iter != items.constEnd())
    {
        (*iter)->setScale(1.0/scaling);
        iter++;
    }
    emit sendMsg(tr("当前视图比例： %1 倍").arg(scaling));
    emit scalingChange(scaling);

}

void CCBView::zoom(qreal proportion)
{
    //    if(abs(proportion-scaling) < 0.001)
    //        return;
    qreal curStep = proportion / scaling;
    scaling = proportion;
    scale(curStep, curStep);

    QList<QGraphicsItem *> items = this->items();
    QList<QGraphicsItem *>::const_iterator iter = items.constBegin();
    while(iter != items.constEnd())
    {
        (*iter)->setScale(1.0/scaling);
        iter++;
    }
    emit sendMsg(tr("当前视图比例： %1 倍").arg(scaling));
    emit scalingChange(scaling);
}

void CCBView::rotateClockwise()
{//jaky,原来是>=
    if(abs(rotation-360.0) <= 0.001)
    {
        return;
    }

    rotation += RSTEP;
    rotate(RSTEP);
    emit rotationChange(rotation);
}

void CCBView::rotateAntiClockwise()
{
    if(abs(rotation-360.0) <= 0.001)
    {
        return;
    }
    rotation -= RSTEP;
    rotate(-RSTEP);
    emit rotationChange(rotation);
}

void CCBView::rotateAngle(qreal angle)
{
    if(abs(angle-rotation) < 0.001)
        return;

    if(angle >= 360.0 || angle <= -360.0)
        return;

    qreal detaAngle = angle - rotation;
    rotation = angle;
    rotate(detaAngle);
    emit rotationChange(rotation);
}
void CCBView::ItemLayerTop(QAction *act)
{
    QList<QGraphicsItem *> cList = scene()->collidingItems(itemAt(RPos));
    cList.push_front(itemAt(RPos));
    ItemBase *b = NULL;
    for(int i=0; i <cList.size();++i)
    {
        b = dynamic_cast<ItemBase*>(cList[i]);
        if(b->getName() == act->text())
        {
            break;
        }
    }
    for(int i = cList.size()-1; i>=0; --i)
        cList.at(i)->stackBefore(b);
    setActivedItem(b);
    update();
}
void CCBView::setActivedItem(QGraphicsItem *value)
{

    if(activedItem == value || value == NULL)
    {
        return;
    }

    if(activedItem != NULL)
    {
        activedItem->setSelected(false);
    }
    activedItem = dynamic_cast<ItemBase*>(value);
    activedItem->setSelected(true);
    //activedItem->setZValue(100);

}
void CCBView::setColorNode(QColor c)
{
    color = c;
}
void CCBView::delNode(ItemBase* it)
{
    if(it == NULL)
    {
        QMessageBox::warning(this, "提示：", "请先选择需要删除的结点");
    }
    else if(it->getLinkerNum() == 0 || it->getLinkerNum() == 1)
    {
        QMessageBox::warning(this, "提示：", "当前选中结点为孤儿结点，不删除");
        return;
    }
    else{//当前item的结构，删除当前item
        it->delItemBase(it);
        //儿子的item结构和父亲的item结构
        QMap<QString, ItemBase*> allItems = Scene::Instance()->getItemContainer();
        QMap<QString, ItemBase*>::const_iterator iter = allItems.constBegin();
        QList<ItemBase*> _s = it->getSon().keys();
        QList<ItemBase*> _list = it->getFather().keys();

        while(iter != allItems.constEnd())
        {
            for(int i=0; i<_s.size();++i)
            {
                if(_s[i]==(*iter))
                {
                    _s[i]->delItemBase(it);//从儿子的item结构中删除it
                }
            }
            for(int j=0; j<_s.size();++j)
            {
                if(_list[j]==(*iter))
                {
                    _list[j]->delItemBase(it);
                }
            }
            ++iter;
        }
        Scene::Instance()->delSceneNode (it);
        update();
    }
}
void CCBView::drawFixedPoint(ItemBase* it)
{
    if(it == NULL)
    {
        QMessageBox::warning(this, "提示：", "请先选择需要寻径的结点");
        emit initCheckBox();
        return;
    }
    else if(it->getLinkerNum() == 0)
    {
        QMessageBox::warning(this, "提示：", "当前选中结点为孤儿结点");
        emit initCheckBox();
        return;
    }
    else
    {
        //隐藏所有ITEM
        QMap<QString, ItemBase*> allItems = Scene::Instance()->getItemContainer();
        QMap<QString, ItemBase*>::const_iterator iter = allItems.constBegin();
        while(iter != allItems.constEnd())
        {
            iter.value()->hide();
            iter++;
        }

        //寻径开始
        //遍历每个结点的邻居结点，如果该结点不在亲戚缓存中，则将ta加入亲戚中，并把ta的邻居结点加入到邻居缓存中
        //如果邻居缓存为空，则停止遍历
        int level = 1;
        int length = 300.0;
        qreal a = 24.0*3.14159265/180;//角度基数
        bool upOrdown = true;
        qreal orgX = -viewport()->width()/2.0;
        qreal orgY = 0.0;

        //以下代码修改于原LINKER结构，为简化实现，先找儿子，再找爹。后续可以改为一次将儿子和爹全找完
        QList<ItemBase*> neighborList = it->getSon().keys();//邻居结点缓存
        QList<ItemBase*> cacheList;
        QList<ItemBase*> linkerList;//全部亲戚结点缓存
        linkerList.append(it);
        it->show();
        while(neighborList.size() > 0)
        {
            QList<ItemBase*>::Iterator iter = neighborList.begin();
            int n = 0;
            while(iter != neighborList.end())
            {
                if(!linkerList.contains(*iter))
                {
                    qreal x = qAbs(length*level*qCos(n*a));
                    qreal y = qAbs(length*level*qSin(n*a));
                    if(upOrdown)//true 为上，false为下
                    {
                        y = -y;
                        n++;
                    }
                    upOrdown = !upOrdown;

                    (*iter)->setPos(orgX+x, orgY+y);
                    (*iter)->show();
                    linkerList.append(*iter);
                    cacheList += (*iter)->getSon().keys();
                    cacheList += (*iter)->getFather().keys();
                }
                iter++;
            }
            if(n > 0)
            {
                level++;
            }
            neighborList.clear();
            neighborList = cacheList;
            cacheList.clear();
        }

        neighborList = it->getFather().keys();//邻居结点缓存
        cacheList.clear();
        linkerList.clear();//全部亲戚结点缓存
        linkerList.append(it);
        level = 1;
        while(neighborList.size() > 0)
        {
            QList<ItemBase*>::Iterator iter = neighborList.begin();
            int n = 0;
            while(iter != neighborList.end())
            {
                if(!linkerList.contains(*iter))
                {
                    qreal x = qAbs(length*level*qCos(n*a));
                    qreal y = qAbs(length*level*qSin(n*a));
                    if(upOrdown)//true 为上，false为下
                    {
                        y = -y;
                        n++;
                    }
                    upOrdown = !upOrdown;

                    (*iter)->setPos(orgX-x, orgY+y);
                    (*iter)->show();
                    linkerList.append(*iter);
                    cacheList += (*iter)->getFather().keys();
                    cacheList += (*iter)->getSon().keys();
                }
                iter++;
            }
            if(n > 0)
            {
                level++;
            }
            neighborList.clear();
            neighborList = cacheList;
            cacheList.clear();
        }

        it->setPos(QPointF(orgX, orgY));
        viewport()->update();
    }
    setActivedItem(it);
}

void CCBView::drawNetBySomeone(bool b)
{
    if(b)
    {
        drawFixedPoint(activedItem);
    }
    else
    {
        QMap<QString, ItemBase*> allItems = Scene::Instance()->getItemContainer();
        QMap<QString, ItemBase*>::const_iterator iter = allItems.constBegin();
        while(iter != allItems.constEnd())
        {
            iter.value()->show();
            iter++;
        }
        setActivedItem(activedItem);
        viewport()->update();
    }
}
void CCBView::routePoint(QString str)
{
    //输入的是客户编号，银行卡号，身份证号，手机号，姓名，目前只能查询客户编号
    //有两个点，根据第一个点开始遍历，保存它的所有路径，去对比第二个点，把终点不是第二个点的去掉
    passPath.clear();

    QMap<QString, ItemBase*> allItems = Scene::Instance()->getItemContainer();
    QMap<QString, ItemBase*>::const_iterator iter = allItems.constBegin();
    ItemBase* _cache = NULL;
    while(iter != allItems.constEnd())
    {
        iter.value()->hide();
        if(iter.key() == str)
            _cache = iter.value();
        ++iter;
    }
    if(_cache == NULL)//没有找到查询结点
    {
        QMessageBox::warning(this, "提示：", "没有找到当前编号的结点，请重新查询");
        return;
    }
    else if(_cache->getLinkerNum() == 0)
    {
        QString s = tr("当前查询结点%1为孤儿结点").arg(str);
        QMessageBox::warning(this, "提示：", s);
        return;
    }
    else
    {
        if(activedItem == NULL)//没有设定活动结点
        {
            QMessageBox::warning(this, "提示：", "请先选择需要寻径的结点");
            return;
        }
        else if(activedItem->getLinkerNum() == 0)
        {
            QMessageBox::warning(this, "提示：", "当前选中结点为孤儿结点");
            return;
        }
        else
        {//有邻居结点，就是有儿子和爹  保存结点路径的结构QList<QList<ItemBase*> >  多条路径，包含多个点
            QList<ItemBase* > passItem;
            activedItem->show();
            QList<ItemBase*> neighborList=repeatOut(activedItem->getSon().keys(), activedItem->getFather().keys());
            for(int i=0; i<neighborList.size();++i)
            {
                passItem.clear();
                if(findNeighbourNode(neighborList[i],_cache,passItem))
                {
                    passPath.append(passItem);
                    for(int j=0; j<passItem.size();++j)
                    {
                        passItem[j]->show();
                    }

                }
            }
            if(passPath.isEmpty())
            {
                QString s = tr("当前选中结点%1与%2 没有关系！").arg(activedItem->getCustomerNO()).arg(_cache->getCustomerNO());
                QMessageBox::warning(this, "提示：", s);
            }
        }
    }
}
void CCBView::findNode(QString str)
{
    //获取全部结点,遍历
    QMap<QString, ItemBase*> allItems = Scene::Instance()->getItemContainer();
    QMap<QString, ItemBase*>::const_iterator iter = allItems.constBegin();
    ItemBase* _cache = NULL;
    while(iter != allItems.constEnd())
    {
        if(iter.key() == str)
        {
            _cache = iter.value();
            setActivedItem(_cache);//设置为选定结点
            return ;
        }
        ++iter;
    }
    if(_cache == NULL)
    {
        QString s = tr("没有找到%1结点，或者该结点为孤立结点！").arg(str);
        QMessageBox::warning(this, "提示：", s);
    }
}
QList<ItemBase*> CCBView::repeatOut(QList<ItemBase*> list1, QList<ItemBase*> list2)
{
    QList<ItemBase*> list;
    list = list1;
    for(int i=0; i< list2.size();++i)
    {
        if(!list1.contains(list2[i]))
            list.append(list2[i]);
    }
    return list;
}
bool CCBView::findNeighbourNode(ItemBase* node, ItemBase* fNode, QList<ItemBase*> &pass)
{
    if(node == fNode)
    {
        pass.append(fNode);
        return true;
    }
    else
    {
        if(node->getLinkerNum() != 0)
        {
            if(pass.contains(node))
                return false;
            pass.append(node);
            QList<ItemBase*> cacheList = repeatOut(node->getSon().keys(), node->getFather().keys());
            for(int i=0; i<cacheList.size();++i)
            {
                findNeighbourNode(cacheList[i],fNode, pass);
            }
        }
    }
    return false;
}
void CCBView::reflush(int)
{
    viewport()->update();
}

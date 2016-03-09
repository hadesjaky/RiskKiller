#include "itembase.h"
#include "scene.h"
#include <QPainter>
#include <QtCore/qmath.h>
#include <QDebug>

int ItemBase::showLinkerNum = 1;
bool ItemBase::topLevelColor = false;
int ItemBase::fontSize = 11.5;
QMap<QString, bool> ItemBase::showFlags;

ItemBase::ItemBase(QObject *parent)
{
    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsFocusable);

    showLinkerNum = 1;
    selected = false;
    color = Qt::gray;
}
bool ItemBase::operator !=(const ItemBase& other) const
{
    return true;
}
void ItemBase::paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget *widget)
{
    if (getLinkerNum()<showLinkerNum)
    {
        return;
    }

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);

    brush.setColor(isSelected() ? Qt::green : color);
    if(_data.getMinLevel() == "1" && topLevelColor)
    {
        brush.setColor(Qt::darkCyan);
    }
    if(StateManager::stateIns()->colorBoard)//调色板存在时，brush再设置一遍颜色
    {
        brush.setColor(color);
    }
    painter->setBrush(brush);

    QFont font;
    font.setPixelSize(fontSize);
    painter->setFont(font);

    QPen pen;
    pen.setStyle(Qt::DotLine);
    pen.setColor(brush.color());
    painter->setPen(pen);

    painter->drawEllipse(0,0,fontSize*2,fontSize*2);
    painter->translate(fontSize,fontSize);

    /*
    painter->drawEllipse(QPointF(2.0*fontSize, 0), fontSize/2, fontSize/2);
    painter->drawLine(QPointF(0,0), QPointF(2.0*fontSize, 0));
    for(int i = 0; i < 5; i++)
    {
        painter->rotate(60);
        painter->drawEllipse(QPointF(2.0*fontSize, 0), fontSize/2, fontSize/2);
        painter->drawLine(QPointF(0,0), QPointF(2.0*fontSize, 0));
    }
    painter->rotate(60);
    */

    pen.setColor(Qt::gray);
    painter->setPen(Qt::gray);
    brush.setColor(Qt::gray);
    painter->setBrush(brush);
    int n = 0;
    if(isSelected())
    {
        painter->drawText(QPointF(fontSize*2,n++*fontSize), tr("姓名：")+_data.getName());
        painter->drawText(QPointF(fontSize*2,n++*fontSize), tr("证件号：")+_data.getId());
        painter->drawText(QPointF(fontSize*2,n++*fontSize), tr("客户编号：%1").arg(_data.getCustomerNO()));
        painter->drawText(QPointF(fontSize*2,n++*fontSize), QString("邻居数：%1").arg(getLinkerNum()));
        painter->drawText(QPointF(fontSize*2,n++*fontSize), tr("银行卡号：%1").arg(_data.getCardNO().join(",")));
        painter->drawText(QPointF(fontSize*2,n++*fontSize), tr("手机号：%1").arg(_data.getMobile().join(",")));
        painter->drawText(QPointF(fontSize*2,n++*fontSize), tr("最小层级：%1").arg(_data.getMinLevel()));

    }
    else
    {
        QMap<QString, bool>::const_iterator iter = showFlags.constBegin();
        while(iter != showFlags.constEnd())
        {
            if(iter.value())
            {
                QString boxName = iter.key();
                QString msg;
                if(tr("%1").arg(NAME_A) == boxName)
                {
                    msg = tr("姓名：%1").arg(_data.getName());
                }
                else if(tr("%1").arg(ADDRESSBOX) == boxName)
                {
                    msg = tr("住址：%1").arg(_data.getAddress());
                }
                else if(tr("%1").arg(MOBILE_A) == boxName)
                {
                    msg = tr("手机：%1").arg(_data.getMobile().join(","));
                }
                else if(tr("%1").arg(CUSTOMER_NO_A) == boxName)
                {
                    msg = tr("客户编号：%1").arg(_data.getCustomerNO());
                }
                else if(tr("%1").arg(LEVEL_A) == boxName)
                {
                    msg = tr("最小层级：%1").arg(_data.getMinLevel());
                }
                else if(tr("%1").arg(LINKER_NUM_A) == boxName)
                {
                    msg = tr("邻居数：%1").arg(getLinkerNum());
                }
                else if(tr("%1").arg(BANK_ADDR_A) == boxName)
                {
                    msg = tr("开户地址：%1").arg(_data.getAddress());
                }
                else if(tr("%1").arg(CARD_NO_A) == boxName)
                {
                    msg = tr("银行卡号：%1").arg(_data.getCardNO().join(","));
                }
                else if(tr("%1").arg(ID_A) == boxName)
                {
                    msg = tr("身份证：%1").arg(_data.getId());
                }
                else if(tr("%1").arg(EMAIL_A) == boxName)
                {
                    msg = tr("EMAIL：%1").arg(_data.getEmail().join(","));
                }
                painter->drawText(QPointF(fontSize*2,0+n++*fontSize), msg);
            }
            iter++;
        }
    }

    //draw lines

    if(!showFlags.value(tr("%1").arg(HIDE_PATH_A)))
    {
//        if(this->son.isEmpty() && father.isEmpty())
//        {
//            return;
//        }
        QMap<ItemBase*, QString>::ConstIterator iter = son.constBegin();
        QList<QString> tmpList = showFlags.keys();

        while(iter != son.constEnd())
        {
            ItemBase *son = iter.key();
            //加个分隔符
            QStringList _flag = iter.value().split("|");

            for(int j=0;j<_flag.size();++j)
            {
                if((son->getLinkerNum() >= showLinkerNum) && son->isVisible() && showFlags.value(_flag[j]))//jaky,去掉showFlags.value(iter.value()
                {
                    QLineF  line = QLineF(QPointF(0,0),mapFromScene(son->pos()));
                    qreal degree = line.angle();
                    painter->rotate(-degree);
                    painter->drawLine(0, 0, line.length()-fontSize, 0);
                    painter->translate(line.length()-fontSize, 0);
                    painter->rotate(15);
                    painter->drawLine(0,0,-fontSize, 0);
                    painter->rotate(-30);
                    painter->drawLine(0,0,-fontSize, 0);
                    painter->rotate(15);
                    painter->translate(-line.length()+fontSize, 0);
                    painter->rotate(degree);
                }
            }
            iter++;
        }
    }
}


QRectF	ItemBase::boundingRect() const
{
    return QRectF(0, 0, fontSize*2, fontSize*2);
}
int ItemBase::getFontSize()
{
    return fontSize;
}

void ItemBase::setFontSize(int value)
{
    fontSize = value;
}
void ItemBase::mergeMethod(ItemBase *fir,ItemBase *sec)
{
    fir->addPerson.append(sec->_data);
    fir->addPerson += sec->addPerson;
    //把sec的儿和父都装进fir中
    QMap<ItemBase*, QString>::ConstIterator iter = sec->son.constBegin();
    while(iter != sec->son.constEnd())
    {
        fir->son.insert(iter.key(),iter.value());
        ++iter;
    }

    QMap<ItemBase*, QString>::ConstIterator iterf = sec->father.constBegin();
    while(iterf != sec->father.constEnd())
    {
        fir->father.insert(iterf.key(),iterf.value());
        ++iterf;
    }
//遍历总的item，找到sec的儿和父的item，把sec的儿中插入fir，父也是
    QMap<QString, ItemBase*> allItems = Scene::Instance()->getItemContainer();
    QMap<QString, ItemBase*>::const_iterator itert = allItems.constBegin();
    QList<ItemBase*> _s = sec->son.keys();
    QList<ItemBase*> _list = sec->father.keys();

    while(itert != allItems.constEnd())
    {
        for(int i=0; i<_s.size();++i)
        {
            if(_s[i]==(*itert))
            {
                _s[i]->son.insert(fir,_s[i]->son.value(sec));
                _s[i]->son.remove(sec);
            }
        }
        for(int j=0; j<_s.size();++j)
        {
            if(_list[j]==(*itert))
            {
                _list[j]->father.insert(fir,_list[j]->father.value(sec));
                _list[j]->father.remove(sec);
            }
        }
        ++itert;
    }
    sec->son.clear();
    sec->father.clear();
}

bool ItemBase::getTopLevelColor()
{
    return topLevelColor;
}

void ItemBase::setTopLevelColor(bool value)
{
    topLevelColor = value;
}

int ItemBase::getShowLinkerNum()
{
    return showLinkerNum;
}

void ItemBase::setShowLinkerNum(int value)
{
    showLinkerNum = value;
}


int ItemBase::getLinkerNum()
{
    //return _data.getLinker().size();
    int num = 0;
    for(int i=0;i<addPerson.size();++i)
    {
        num += addPerson[i].getSonNum() + addPerson[i].getFatherNum();
    }
    num += _data.getSonNum() + _data.getFatherNum();
    return num;
}

int ItemBase::getSonNum()
{
    return getLinkerNum();
}

QString ItemBase::getId() const
{
    //return _data.getId();
    QStringList s;
    s << _data.getId() ;
    for(int i=0; i <addPerson.size(); ++i)
    {
        s << addPerson[i].getId();
    }
    return s.join(',');
}

QString ItemBase::getName() const
{
    QStringList s;
    s << _data.getName() ;
    for(int i=0; i <addPerson.size(); ++i)
    {
        s << addPerson[i].getName();
    }
    return s.join(',');
}

QStringList ItemBase::getCardNO() const
{
    //return _data.getCardNO();
    QStringList s;
    s << _data.getCardNO() ;
    for(int i=0; i <addPerson.size(); ++i)
    {
        s << addPerson[i].getCardNO();
    }
    return s;
}
QStringList ItemBase::getMobile() const
{
    //return _data.getMobile();
    QStringList s;
    s << _data.getMobile() ;
    for(int i=0; i <addPerson.size(); ++i)
    {
        s << addPerson[i].getMobile();
    }
    return s;
}
QStringList ItemBase::getLinkType() const
{
    //return _data.getLinkType();
    QStringList s;
    s << _data.getLinkType() ;
    for(int i=0; i <addPerson.size(); ++i)
    {
        s << addPerson[i].getLinkType();
    }
    return s;
}
QStringList ItemBase::getEmail() const
{
    //return _data.getEmail();
    QStringList s;
    s << _data.getEmail() ;
    for(int i=0; i <addPerson.size(); ++i)
    {
        s << addPerson[i].getEmail();
    }
    return s;
}
QStringList ItemBase::getLevel() const
{
    //return _data.getLevel();
    QStringList s;
    s << _data.getLevel() ;
    for(int i=0; i <addPerson.size(); ++i)
    {
        s << addPerson[i].getLevel();
    }
    return s;
}

QString ItemBase::getMinLevel()
{
    //return _data.getMinLevel();
    QStringList s;
    s << _data.getMinLevel() ;
    for(int i=0; i <addPerson.size(); ++i)
    {
        s << addPerson[i].getMinLevel();
    }
    return s.join(',');
}

QString ItemBase::getCustomerNO() const
{
    //return _data.getCustomerNO();
    QStringList s;
    s << _data.getCustomerNO() ;
    for(int i=0; i <addPerson.size(); ++i)
    {
        s << addPerson[i].getCustomerNO();
    }
    return s.join(',');
}

QMap<ItemBase *, QString> ItemBase::getLinker() const
{
    return linker;
}

QMap<ItemBase *, QString> ItemBase::getSon() const
{
    return son;
}

QMap<ItemBase *, QString> ItemBase::getFather() const
{
    return father;
}

void ItemBase::addLinker(ItemBase *item, QString type)
{
    linker.insert(item, type);
    LogicPerson p = item->getLogicPerson();
    _data.addLinker(&p, type);
}

void ItemBase::addSon(ItemBase *item, QString type)
{
    son.insert(item, type);
    LogicPerson p = item->getLogicPerson();
    _data.addSon(&p, type);
}

void ItemBase::addFather(ItemBase *item, QString type)
{
    father.insert(item, type);
    LogicPerson p = item->getLogicPerson();
    _data.addFather(&p, type);
}
void ItemBase::delItemBase(ItemBase *it)
{
    //_data.delPerson(&it->_data);
    this->son.remove(it);
    this->father.remove(it);
}
LogicPerson ItemBase::getPerson() const
{
    return _data;
}
void ItemBase::bindLinker()
{
    QMap<LogicPerson*, QString>::ConstIterator sonIter = _data.getSon().constBegin();
    while(sonIter != _data.getSon().constEnd())
    {
        ItemBase *item;
        if( Scene::Instance()->getItemByID(sonIter.key()->getCustomerNO()) )
        {
            item = Scene::Instance()->getItemByID(sonIter.key()->getCustomerNO()) ;
        }
        else if( Scene::Instance()->getItemByID(sonIter.key()->getId()) )
        {
            item = Scene::Instance()->getItemByID(sonIter.key()->getId()) ;
        }
        else if( Scene::Instance()->getItemByID(sonIter.key()->getCardNO().first()) )
        {
            item = Scene::Instance()->getItemByID(sonIter.key()->getCardNO().first()) ;
        }
        son.insert(item, sonIter.value());
        sonIter++;
    }

    QMap<LogicPerson*, QString>::ConstIterator iter = _data.getFather().constBegin();
    while(iter != _data.getFather().constEnd())
    {
        ItemBase *item;
        if( Scene::Instance()->getItemByID(iter.key()->getCustomerNO()) )
        {
            item = Scene::Instance()->getItemByID(iter.key()->getCustomerNO()) ;
        }
        else if( Scene::Instance()->getItemByID(iter.key()->getId()) )
        {
            item = Scene::Instance()->getItemByID(iter.key()->getId()) ;
        }
        else if( Scene::Instance()->getItemByID(iter.key()->getCardNO().first()) )
        {
            item = Scene::Instance()->getItemByID(iter.key()->getCardNO().first()) ;
        }
        father.insert(item, iter.value());
        iter++;
    }

}

QString ItemBase::getType() const
{
    return _data.getType();
}

int ItemBase::getFatherNum()
{
    return getLinkerNum();
}


QPointF ItemBase::getOrgPos()
{
    return orgPos;
}

LogicPerson ItemBase::getLogicPerson()
{
    return _data;
}

void ItemBase::bindLogicPerson(LogicPerson d)
{
    _data = d;
}

void ItemBase::changeShowFlag(QString attr, bool flag)
{
    showFlags.insert(attr, flag);
}

void ItemBase::setSelected(bool b)
{
    selected = b;
}

bool ItemBase::isSelected()
{
    return selected;
}
void ItemBase::setItemColor(QColor c)
{
    color = c;
}

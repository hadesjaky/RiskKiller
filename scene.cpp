#include "scene.h"
#include "datareader.h"
#include <QDir>
#include <QtAlgorithms>
#include <QList>
#include <QtMath>
#include <QVector>
#include<typeinfo>
#include <QGraphicsView>
#include "xmlFile.h"
#include "readFileBase.h"




Scene* Scene::_instance = 0;
int Scene::nodeInterval = 100;
pthread_mutex_t Scene::mutex_delSceneItem = PTHREAD_MUTEX_INITIALIZER;
DrawMethod Scene::nowMethod = CYCLE;

Scene *Scene::Instance()
{
    if (_instance == 0)
    {
        _instance = new Scene();
    }
    return _instance;
}
QMap<QString, ItemBase *> Scene::getItemContainer() const
{
    return itemContainer;
}

Scene::Scene(QObject *parent) :
    QGraphicsScene(parent)
{
    //createRelation();
}
void Scene::init()
{
    if(!itemContainer.isEmpty())
    {
        drawPaths(CYCLE);
    }
    update();
}
void Scene::clearAll()
{
    People::Instance()->clearAll();
    QMap<QString, ItemBase*>::const_iterator it = itemContainer.constBegin();
    while(it!= itemContainer.constEnd())
    {
        delete it.value();
        it++;
    }
    itemContainer.clear();

}
void Scene::delSceneNode(ItemBase *it)
{
    QString s = itemContainer.key(it);
    if(s.isEmpty())
    {
        return;
    }
    itemContainer.remove(s);
    //it->hide();
    drawPaths(CYCLE);
    QList<QGraphicsView *> views = this->views();
    QList<QGraphicsView *>::const_iterator iter = views.constBegin();
    while(iter != views.constEnd())
    {
        (*iter)->viewport()->update();
        iter++;
    }

}
void Scene::updateProgress(QString msg)
{
    emit sendMsg(msg);
}
ItemBase *Scene::getActivedItem() const
{
    return activedItem;
}

void Scene::setActivedItem(ItemBase *value)
{
    activedItem = value;
}


void Scene::drawPaths(int m)
{
    QMap<QString, ItemBase*>::const_iterator i = itemContainer.constBegin();
    nowMethod =(DrawMethod)m;
    if(m ==  LEVEL) //按层展示
    {
        QMap<int,int> posNonius;//位置游标QMap<层级, 层内位置>

        while(i != itemContainer.constEnd())
        {
            ItemBase* someBody = i.value();

            int level = someBody->getMinLevel().toInt();
            if(!posNonius.contains(level))
            {
                posNonius.insert(level, 0);
            }
            else
            {
                posNonius[level]++;
            }
            someBody->setPos(40*posNonius[level]+rand()%5,100*(level-1)+rand()%5);
            i++;

        }
    }
    else if(m == CYCLE)//根据邻居结点数量画圆圈
    {
        QMultiMap< int, ItemBase* > itemListByNeighbour;//根据邻居结点数量进行归类，QMap<邻居数，结点集合>
        while(i != itemContainer.constEnd())
        {
            ItemBase *item = i.value();
            itemListByNeighbour.insertMulti(item->getLinkerNum(), item);
            i++;
        }

        //绘图
        QList<int> neighbourKeys = itemListByNeighbour.uniqueKeys();
        qSort(neighbourKeys);
        QList<int>::const_iterator keyIter = neighbourKeys.constEnd();
        int cycleNum = 1;//圈数
        int r = 20;//半径增量
        int detar = 30;//内圈半径
        //处理第cycleNum圈图元
        while(--keyIter >= neighbourKeys.constBegin())
        {
            QList<ItemBase*> manyItems = itemListByNeighbour.values(*keyIter);
            QList< ItemBase* >::const_iterator innerIter = manyItems.constBegin();
            int n = 0;//角度片数
            qreal a = 2*3.14159265/manyItems.size();//角度基数
            while(innerIter != manyItems.constEnd())
            {
                ItemBase *item = *innerIter;
                qreal x = (detar + nodeInterval + cycleNum*r)*qCos(n*a);
                qreal y = (detar + nodeInterval + cycleNum*r)*qSin(n*a);
                item->setPos(x, y);
                innerIter++;
                n++;
            }
            cycleNum++;
        }

    }

    QList<QGraphicsView *> views = this->views();
    QList<QGraphicsView *>::const_iterator iter = views.constBegin();
    while(iter != views.constEnd())
    {
        (*iter)->viewport()->update();
        iter++;
    }

}
void Scene::nodeFilter(QString flag, bool b)
{
    QList<ItemBase*> aList = itemContainer.values();  //所有的结点
    for(int i = 0; i<aList.size(); ++i)
    {//遍历每个点的子节点集合，结构<结点，关系>
        QMap<ItemBase *, QString> _sonMap = aList[i]->getSon();
        QMap<ItemBase*, QString>::ConstIterator iter = _sonMap.constBegin();
        while (iter != _sonMap.constEnd())
        {
            ItemBase *_s = iter.key();
            QStringList _flag = iter.value().split("|");
            if(_flag.contains(flag) && b)//结构中包含关系
                _s->show();
            else if(_flag.contains(flag) && !b)//包含，判断界面中是否有选中关系，有选中再判断
            {
                _s->hide();
                QMap<QString,bool>::ConstIterator ite = StateManager::stateIns()->showFlagsMan.constBegin();
                while (ite != StateManager::stateIns()->showFlagsMan.constEnd()) {
                    if(ite.value())
                    {
                        if(_flag.contains(ite.key()))
                            _s->show();
                    }
                    ite++;
                }
            }
            iter++;
        }
    }
}
void Scene::createItems()
{
    QVector<LogicPerson*> people = People::Instance()->getStaff();
    QVector<LogicPerson*>::const_iterator iter = people.constBegin();

    emit sendMsg(tr("开始创建客户图元"));
    while(iter != people.constEnd())
    {
        //创建图元，并将逻辑层数据附在图元上。
        if((*iter)->getLinkerNum() == 0)
        {
            iter++;
            continue;
        }

        ItemBase *item = new ItemBase(this);
        LogicPerson *p = *iter;
        item->bindLogicPerson(*p);
        addItem(item);
        iter++;

    }
    emit sendMsg(tr("完成客户图元创建"));

    //处理图元linker
    emit sendMsg(tr("开始创建人物关系"));
    QMap<QString, ItemBase*>::ConstIterator linkerIter = itemContainer.constBegin();
    while(linkerIter != itemContainer.constEnd())
    {
        if(linkerIter.value()->getLinkerNum() != 0)
        {
            linkerIter.value()->bindLinker();
        }
        linkerIter++;
    }

    drawPaths(CYCLE);

    emit sendMsg(tr("打开文件成功！当前人物总数为")+QString("%1").arg(itemContainer.size()));
}

void Scene::addItem(ItemBase* item)
{

    QString id;
    if(!item->getCustomerNO().isEmpty())
    {
        id = item->getCustomerNO();
    }
    else if(!item->getId().isEmpty())
    {
        id = item->getId();
    }
    else if(item->getCardNO().size() > 0)
    {
        id = item->getCardNO().first();
    }
    itemContainer.insert(id, item);
    QGraphicsScene::addItem(item);
}

void Scene::addItem(QGraphicsItem* item)
{
    QGraphicsScene::addItem(item);
}

void Scene::loadData(ReadFileBase *file)
{
    if(typeid(*file) == typeid(XmlFile))
    {
        XmlFile *in = dynamic_cast<XmlFile*>(file);
        emit sendMsg(tr("开始导入后台数据"));
        connect(in, SIGNAL(loadFinished()), this, SLOT(createItems()));
        connect(in, SIGNAL(updateProgress(QString)), this, SLOT(updateProgress(QString)));
        in->start();
    }
    else
    {
        DataReader *in = dynamic_cast<DataReader*>(file);
        emit sendMsg(tr("开始导入后台数据"));
        connect(in, SIGNAL(loadFinished()), this, SLOT(createItems()));
        connect(in, SIGNAL(updateProgress(QString)), this, SLOT(updateProgress(QString)));
        in->start();
    }

}

void Scene::loadTestData()
{
    //loadData("D:\\project-space\\testdata\\testdata.dat");
}

ItemBase *Scene::getItemByID(QString i)
{
    return itemContainer.value(i);
}

void Scene::createRelation()
{
    QMap<QString, ItemBase*>::const_iterator i = itemContainer.constBegin();
    while(i != itemContainer.constEnd())
    {
        ItemBase *itemFather = i.value();
        QString fid = itemFather->getCustomerNO();
        QMap<ItemBase*, QString> son = itemFather->getLinker();
        QMap<ItemBase*, QString>::const_iterator iSon = son.constBegin();
        while(iSon != son.constEnd())
        {
            ItemBase *itemSon = iSon.key();
            if(itemSon != NULL)
            {
                addLine(QLineF(itemFather->pos(),itemSon->pos()));
            }

            iSon++;
        }
        i++;
    }

}

void Scene::changeLine()
{
    createRelation();
}

void Scene::hideItemBySonNum(int n=0)
{
    QMap<QString, ItemBase*>::const_iterator i = itemContainer.constBegin();
    while(i != itemContainer.constEnd())
    {
        ItemBase *item = i.value();
        if(item->getSonNum() <= n)
        {
            item->hide();
        }
        else
        {
            item->show();
        }
        i++;
    }

}

void Scene::hideItemByFatherNum(int n=0)
{
    QMap<QString, ItemBase*>::const_iterator i = itemContainer.constBegin();
    while(i != itemContainer.constEnd())
    {
        ItemBase *item = i.value();
        if(item->getFatherNum() <= n)
        {
            item->hide();
        }
        else
        {
            item->show();
        }
        i++;
    }
}
void Scene::setNodeInterval(int value)
{
    if(value <= 0)
        return;
    nodeInterval = value;
    drawPaths(nowMethod);
}
void Scene::relationShow(QString str, bool b)
{

}

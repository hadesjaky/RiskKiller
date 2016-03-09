#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include "itembase.h"
#include <QMap>
#include <QGraphicsSceneMouseEvent>
#include <pthread.h>
#include "globaldef.h"
#include "readFileBase.h"

//唯一模式

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    static Scene* Instance();


    QMap<QString, ItemBase *> getItemContainer() const;

    ItemBase *getActivedItem() const;
    void setActivedItem(ItemBase *value);
    void init();//初始化
    void clearAll();//清空itermContainter，itermBase
    void delSceneNode(ItemBase *it);

private:
    QMap<QString, ItemBase*> itemContainer;//可以是客户编号，身份证号，卡号, item
    QMap<QString, QGraphicsLineItem*> lines;
    explicit Scene(QObject *parent = 0);
    static Scene* _instance;
    ItemBase *activedItem;
    static pthread_mutex_t mutex_delSceneItem;
    static int nodeInterval;//当前的结点间距
    static DrawMethod nowMethod;//保存当前的显示方式

signals:
    void sendMsg(QString msg);
public slots:
    void updateProgress(QString msg);
    void addItem(ItemBase *item);
    void addItem(QGraphicsItem *item);
    void loadData(ReadFileBase *file);
    void loadTestData();//测试用
    ItemBase* getItemByID(QString );
    void createRelation();//创建关系
    void changeLine();
    void hideItemBySonNum(int n);
    void hideItemByFatherNum(int n);
    void drawPaths(int method);//绘图
    void nodeFilter(QString flag, bool b);//结点过滤，界面关系展示，结点的显示和隐藏
    void createItems();
    void setNodeInterval(int value);//设置结点间距
    void relationShow(QString str, bool b);//关系选择框内，隐藏未选择的无关的item

};

#endif // SCENE_H

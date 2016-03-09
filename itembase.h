#ifndef ITEMBASE_H
#define ITEMBASE_H

#include <QGraphicsItem>
#include <QColor>
#include "logicperson.h"
#include "statMmanager.h"
#include <pthread.h>

class ItemBase : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit ItemBase(QObject *parent = 0);
    bool operator !=(const ItemBase& other) const;
    int getLinkerNum();//获得邻居节点数，儿子和爹的数
    int getFatherNum();
    int getSonNum();

    //封装
    QString getId() const;
    QString getName() const;
    QStringList getCardNO() const;
    QStringList getMobile() const;
    QStringList getLinkType() const;
    QStringList getEmail() const;
    //QStringList getDetailInfo() const;
    QStringList getLevel() const;
    QString getMinLevel();
    QString getCustomerNO() const;

    QString getType() const;

    static int getShowLinkerNum();
    static void setShowLinkerNum(int value);

    static bool getTopLevelColor();
    static void setTopLevelColor(bool value);

    static int getFontSize();
    static void setFontSize(int value);

//    static int getNodeInterval();
//    static void setNodeInterval(int value);
    static void mergeMethod(ItemBase *fir,ItemBase *sec);//将sec放入到fir中，合并

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget *widget);
    QRectF	boundingRect() const;


private:
    QPointF orgPos;
    LogicPerson _data;
    QList<LogicPerson> addPerson;//后合并的人
    static int showLinkerNum;
    static bool topLevelColor;
    static int fontSize;
    static int nodeInterval;
    static QMap<QString, bool> showFlags;//初始创建界面时候分配的
    QMap<ItemBase*, QString> linker;// item,关系类型，多关系用“|”分开,没调用
    QMap<ItemBase*, QString> son;
    QMap<ItemBase*, QString> father;
    bool selected;
    QColor color;
signals:

public slots:
    QPointF getOrgPos();
    void bindLogicPerson(LogicPerson d);
    LogicPerson getLogicPerson();
    QMap<ItemBase *, QString> getLinker() const;
    QMap<ItemBase *, QString> getSon() const;
    QMap<ItemBase *, QString> getFather() const;

    void addLinker(ItemBase *item, QString type);
    void addSon(ItemBase *item, QString type);
    void addFather(ItemBase *item, QString type);

    void delItemBase(ItemBase *it);//删除it结构内的关系

    LogicPerson getPerson() const;
    void bindLinker();//调用的这个创建父子关系
    static void changeShowFlag(QString, bool);
    void setSelected(bool b);
    bool isSelected();
    void setItemColor(QColor c);
};

#endif // ITEMBASE_H

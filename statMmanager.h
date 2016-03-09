#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <QObject>
#include <QMap>
/*
状态管理类
*/

class StateManager : public QObject
{
    Q_OBJECT
public:
    static StateManager *stateIns();
    bool colorBoard;//调色板
    bool hiddenNodeAct;//隐藏
    bool delNodeAct;//删除
    bool selectModel;//选择或拖拽,选择模式下，区域选择会随鼠标移动刷新
    bool mergeNodeAct;//合并
//<关系选择框>的状态
//    bool moneyRelation;//资金关系
//    bool idRelation;//证件关系
//    bool addrRelation;//地址关系
//    bool societyRelation;//社工关系
//    bool IPRelation;//IP关系
//    bool phoneRelation;//手机号
//    bool MacRelation;//Mac地址
//    bool ATMRelation;//ATM号
    QMap<QString, bool> showFlagsMan;

signals:

public slots:
private:
    explicit StateManager(QObject *parent = 0);
    ~StateManager();
    static StateManager *state;
};

#endif // STATEMANAGER_H

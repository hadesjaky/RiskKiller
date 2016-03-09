#ifndef LOGICPERSON_H
#define LOGICPERSON_H

#include "globaldef.h"
#include <QStringList>
#include <QVector>
#include <QMap>
#include <QString>
#include <pthread.h>

class LogicPerson
{
private:
    QString id;//身份证
    QString name;
    QString customerNO;//客户编号
    QStringList cardNO;//卡号
    QStringList mobile;
    QStringList dev;//设备
    QStringList detailInfo;
    QStringList email;
    QString type;//行内为ccb，行外为other
    QString address;

    QMap<LogicPerson*, QString> linker;//邻居指针，关系类别，
    QMap<LogicPerson*, QString> son;
    QMap<LogicPerson*, QString> father;
    QStringList level;

public:
    LogicPerson();
    LogicPerson& operator = (const LogicPerson& op);
    bool operator !=(const LogicPerson& other) const;

    QString getId() const;
    void setId(const QString &value);

    QString getName() const;
    void setName(const QString &value);

    QStringList getCardNO() const;
    void addCardNO(const QString &value);
    void addCardNO(const QStringList &value);
    void setCardNO(const QStringList &value);

    QStringList getMobile() const;
    void addMobile(const QString &value);
    void setMobile(const QStringList &value);

    QStringList getLinkType() const;
    void setDev(const QStringList &value);

    QStringList getDetailInfo() const;
    void setDetailInfo(const QStringList &value);

    QStringList getLevel() const;
    void pushLevel(QString);
    QString getMinLevel();



    QString getCustomerNO() const;
    void setCustomerNO(const QString &value);

    void setData(const QString fieldName, QString value);

    QMap<LogicPerson *, QString> getLinker() const;
    void addLinker(LogicPerson *p, QString &value);
    void addSon(LogicPerson *p, QString &value);
    void addFather(LogicPerson *p, QString &value);

    int getLinkerNum() const;
    QString getType() const;
    void setType(const QString &value);
    QStringList getEmail() const;
    void addEmail(const QStringList &value);
    void addEmail(const QString &value);
    QString getAddress() const;
    void setAddress(const QString &value);
    QMap<LogicPerson *, QString> getSon() const;
    QMap<LogicPerson *, QString> getFather() const;

    int getFatherNum();
    int getSonNum();
    void delPerson(LogicPerson* p);
};

class People
{
public:
    static People* Instance();

private:
    People();

    static People* _instance;
    QVector<LogicPerson*> staff;
    QMap<QString, LogicPerson*> roster;//身份证号和人


public:
    void registerPerson(LogicPerson *p);
    bool contains(const QString &k);
    LogicPerson* getPerson(const QString &k);

    QVector<LogicPerson *> getStaff() const;
    void delPerson(QString s);
    void clearAll();//清空所有人
};
#endif // LogicPerson_H

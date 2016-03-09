#include "logicperson.h"
#include <QDebug>

LogicPerson& LogicPerson::operator = (const LogicPerson& op)
{
    (*this).id = op.id;
    (*this).name = op.name;
    (*this).customerNO = op.customerNO;
    (*this).cardNO = op.cardNO;
    (*this).mobile = op.mobile;
    (*this).dev = op.dev;
    (*this).detailInfo = op.detailInfo;
    (*this).email = op.email;
    (*this).type = op.type;
    (*this).address = op.address;
    (*this).linker = op.linker;
    (*this).son = op.son;
    (*this).father = op.father;
    (*this).level = op.level;
    return *this;
}
bool LogicPerson::operator !=(const LogicPerson& other) const
{
    return (this->id == other.id && this->name == other.name && this->customerNO == other.customerNO);
}
QString LogicPerson::getCustomerNO() const
{
    return customerNO;
}

void LogicPerson::setCustomerNO(const QString &value)
{
    customerNO = value;
}

QMap<LogicPerson *, QString> LogicPerson::getLinker() const
{
    return linker;
}

void LogicPerson::addLinker(LogicPerson *p, QString &value)
{
    if(p->getCustomerNO() != getCustomerNO())
    {
        linker.insert(p, value);
    }
}

void LogicPerson::addSon(LogicPerson *p, QString &value)
{
    if(p->getCustomerNO() != getCustomerNO())
    {
        son.insert(p, value);
    }
}

void LogicPerson::addFather(LogicPerson *p, QString &value)
{
    if(p->getCustomerNO() != getCustomerNO())
    {
        father.insert(p, value);
    }
}

int LogicPerson::getLinkerNum() const
{
    //return linker.size();
    return son.size()+father.size();
}


QString LogicPerson::getType() const
{
    return type;
}

void LogicPerson::setType(const QString &value)
{
    type = value;
}

QStringList LogicPerson::getEmail() const
{
    return email;
}

void LogicPerson::addEmail(const QStringList &value)
{
    email += value;
}

void LogicPerson::addEmail(const QString &value)
{
    email.append(value);
}


QString LogicPerson::getAddress() const
{
    return address;
}

void LogicPerson::setAddress(const QString &value)
{
    address = value;
}

QMap<LogicPerson *, QString> LogicPerson::getSon() const
{
    return son;
}

int LogicPerson::getSonNum()
{
    return son.size();
}
void LogicPerson::delPerson(LogicPerson* p)
{
    p->son.remove(p);
    p->father.remove(p);
    //delete p;
}

QMap<LogicPerson *, QString> LogicPerson::getFather() const
{
    return father;
}

int LogicPerson::getFatherNum()
{
    return father.size();
}

LogicPerson::LogicPerson()
{
}

QString LogicPerson::getId() const
{
    return id;
}

void LogicPerson::setId(const QString &value)
{
    id = value;
}
QString LogicPerson::getName() const
{
    return name;
}

void LogicPerson::setName(const QString &value)
{
    name = value;
}
QStringList LogicPerson::getCardNO() const
{
    return cardNO;
}

void LogicPerson::addCardNO(const QString &value)
{
    cardNO.append(value);
}

void LogicPerson::addCardNO(const QStringList &value)
{
    cardNO += value;
}
void LogicPerson::setCardNO(const QStringList &value)
{
    cardNO = value;
}
QStringList LogicPerson::getMobile() const
{
    return mobile;
}

void LogicPerson::setMobile(const QStringList &value)
{
    mobile = value;
}
void LogicPerson::addMobile(const QString &value)
{
    mobile.append(value);
}
QStringList LogicPerson::getLinkType() const
{
    return dev;
}

void LogicPerson::setDev(const QStringList &value)
{
    dev = value;
}
QStringList LogicPerson::getDetailInfo() const
{
    return detailInfo;
}

void LogicPerson::setDetailInfo(const QStringList &value)
{
    detailInfo = value;
}

QStringList LogicPerson::getLevel() const
{
    return level;
}
void LogicPerson::pushLevel(QString l)
{
    level << l;
}

QString LogicPerson::getMinLevel()
{
    level.sort();
    if(level.size() > 0)
        return level.at(0);
    else
        return QString("5");
}

void LogicPerson::setData(const QString fieldName, QString value)
{
    if(fieldName == "level")
    {
        pushLevel(value);
    }
    else if(fieldName == "customer_no" or fieldName == "customerNO")
    {
        setCustomerNO(value);
    }
    else if(fieldName == "card_no" or fieldName == "cardNO")
    {
        addCardNO(value);
    }
    else if(fieldName == "email")
    {
        addEmail(value);
    }
    else if(fieldName == "id")
    {
        setId(value);
    }

    else if(fieldName == "name")
    {
        setName(value);
    }
    else if(fieldName == "mobile")
    {
        addMobile(value);
    }
    //jaky,email重复,以删除，问题：tel和mobile是不是应该归一组
    else if(fieldName == "tel")
    {
        addMobile(value);
    }
}
/********************************************************************/
People* People::_instance = 0;
People *People::Instance()
{
    if (_instance == 0)
    {
        _instance = new People();
    }
    return _instance;
}

People::People()
{

}

QVector<LogicPerson *> People::getStaff() const
{
    return staff;
}

void People::registerPerson(LogicPerson *p)
{
    staff.append(p);
    if(p->getCardNO().size() > 0)//卡号
    {
        for(int i=0; i<p->getCardNO().size(); i++)
        {
            roster.insert(p->getCardNO().at(i), p);
        }
    }

    if(!p->getId().isEmpty())//身份证
    {
        roster.insert(p->getId(), p);
    }

    if(!p->getCustomerNO().isEmpty())//客户编号
    {
        roster.insert(p->getCustomerNO(), p);
    }
}

bool People::contains(const QString &k)
{
    return roster.contains(k);
}

LogicPerson *People::getPerson(const QString &k)
{
    return roster.value(k);
}
void People::delPerson(QString s)
{
    int num = staff.indexOf(roster[s]);
    if(num != -1)
        staff.remove(num);
    else
        qDebug() << "del person is error" << num;
    roster.remove(s);
}
void People::clearAll()
{
    for(int i=0; i<staff.size(); ++i)
        delete staff[i];
    staff.clear();
    roster.clear();
}

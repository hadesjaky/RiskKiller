#include "datareader.h"
#include <QStringList>
#include <QDir>
#include <QDebug>

DataReader::DataReader(QString _fileName):ReadFileBase(_fileName)
{
    curFileName = _fileName;
    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()), this, SLOT(checkProgress()));
    timer->start(200);
}

DataReader::~DataReader()
{

}

void DataReader::loadData()
{
    const QVector<CFGFile*> &CFGContainer = CFGManager::Instance()->getCFGContainer();

    for(int i=0; i<CFGContainer.size(); i++)
    {
        CFGFile* cfg = CFGContainer.at(i);
        if(cfg->getType() == "S")//装载静态数据
        {
            loadSData(cfg);
        }
        else//装载动态数据
        {
            loadDData(cfg);
        }

    }
    timer->stop();
}

void DataReader::loadSData(CFGFile *cfg)
{
    QFile file(cfg->getPath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "打开失败" << cfg->getPath();
        return;

    }
    const QMap<QString, int>& fieldCFG = cfg->getFieldMap();
    QTextStream in(&file);
    QString line;
    QStringList record;

    QList<int> indexList = fieldCFG.values();
    int max = 0;
    for(int i=0; i<indexList.size(); i++)
    {
        max = max<indexList.at(i) ? indexList.at(i) : max;
    }

    ///进度提醒
    totalSize = file.size();
    curSize = 0;
    curFileName = file.fileName().split("/").last();
    lineNO = 0;

    while(!in.atEnd())
    {
        // QTest::qSleep(2000);
        line = in.readLine();
        lineNO++;
        curSize += line.size();
        if(line.isEmpty() || line.isNull() || "#"==line.at(0))
        {
            continue;
        }
        record = splitCSVLine(line);//读取第一行，并分割开
        if(record.size() < max)
        {
            line = in.readLine();
            continue;
        }
        int customerNOIndex = fieldCFG.value("customer_no");
        QString customerNO = record.at(customerNOIndex);//jaky，record是list，坐标从0开始
        if(People::Instance()->contains(customerNO))
        {
            continue;
        }
        LogicPerson *person = new LogicPerson();
        QMap<QString, int>::const_iterator iter = fieldCFG.constBegin();
        while(iter != fieldCFG.constEnd())
        {
            QString value = record.at(iter.value());
            QString key = iter.key();
            person->setData(key, value);
            iter++;
        }
        People::Instance()->registerPerson(person);
    }
    file.close();
}
void DataReader::loadDData(CFGFile *cfg)
{
    QFile file(cfg->getPath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << cfg->getPath() << "failed";
        return;
    }
    const QMap<QString, int>& fieldCFG = cfg->getFieldMap();
    QTextStream in(&file);
    QString line;
    QStringList record;

    QList<int> indexList = fieldCFG.values();
    int max = 0;
    for(int i=0; i<indexList.size(); i++)
    {
        max = max<indexList.at(i) ? indexList.at(i) : max;
    }

    ///进度提醒
    totalSize = file.size();
    curSize = 0;
    curFileName = file.fileName().split("/").last();
    lineNO = 0;
    while(!in.atEnd())
    {
        line = in.readLine();
        lineNO++;
        curSize += line.size();
        if(line.isEmpty() || line.isNull() || "#"==line.at(0))
        {
            continue;
        }

        record = splitCSVLine(line);

        if(record.size() < max)
        {
            continue;
        }
        QString sonCustomerNO = record.at(fieldCFG.value("son_customer_no") );//jaky,
        QString sonCardNO = record.at(fieldCFG.value("son_card_no"));
        QString linkType = record.at(fieldCFG.value("link_type"));
        if(sonCustomerNO.isEmpty() && sonCardNO.isEmpty())
        {
            continue;
        }

        int customerNOIndex = fieldCFG.value("customer_no");
        QString customerNO = record.at(customerNOIndex);
        if(!People::Instance()->contains(customerNO))
        {
            continue;
        }
        LogicPerson *masterCust = People::Instance()->getPerson(customerNO);
        QString level = record.at(fieldCFG.value("level"));
        masterCust->pushLevel(level);

        QString loanNum = record.at(fieldCFG.value("loan_flag"));
        if(!sonCustomerNO.isEmpty())
        {
            LogicPerson* slaveCust = People::Instance()->getPerson(sonCustomerNO);
            if(slaveCust != NULL)
            {
                if(loanNum != "0.00")//支出，主账号给辅账号转钱，主账号是爹
                {
                    masterCust->addSon(slaveCust, linkType);
                    slaveCust->addFather(masterCust, linkType);
                }
                else
                {
                    masterCust->addFather(slaveCust, linkType);
                    slaveCust->addSon(masterCust, linkType);
                }
            }

        }

        else if(sonCustomerNO.isEmpty() && !sonCardNO.isEmpty())//他行客户需要新建对象
        {
            LogicPerson* otherBankCust = People::Instance()->getPerson(sonCardNO);
            if(otherBankCust == NULL)
            {
                otherBankCust = new LogicPerson();
                otherBankCust->addCardNO(sonCardNO);
                otherBankCust->setType(sonCardNO);
                People::Instance()->registerPerson(otherBankCust);
            }
            if(loanNum == "0.00")//支出，主账号给辅账号转钱，主账号是爹
            {
                masterCust->addSon(otherBankCust, linkType);
                otherBankCust->addFather(masterCust, linkType);
            }
            else
            {
                masterCust->addFather(otherBankCust, linkType);
                otherBankCust->addSon(masterCust, linkType);
            }
        }
    }
    file.close();
}

void DataReader::run()
{
    const QVector<CFGFile*> &CFGContainer = CFGManager::Instance()->getCFGContainer();

    for(int i=0; i<CFGContainer.size(); i++)
    {
        CFGFile* cfg = CFGContainer.at(i);
        if(cfg->getType() == "S")
        {
            loadSData(cfg);
        }
        else
        {
            loadDData(cfg);
        }

    }

    connect(this, SIGNAL(loadFinished()), this, SLOT(stopTimer()));
    emit loadFinished();
}

void DataReader::checkProgress()
{
    emit updateProgress(tr("正在导入文件:%1,进度%2 %, 当前行数：%3").arg(curFileName).arg(1.0*curSize/totalSize*100).arg(lineNO));
}

void DataReader::stopTimer()
{
    timer->stop();
}




QStringList DataReader::splitCSVLine(const QString &line)
{
    QStringList strList;
    QString str;

    int length = line.length();
    int quoteCount = 0;
    bool insideFlag = false;


    for(int i = 0; i < length; i++)
    {


        if(line[i] == '"')//in or out
        {
            if(quoteCount++ == 0)
            {
                insideFlag = true; //in
                continue;
            }


            if(insideFlag && quoteCount%2==0)//out
            {
                if(i == length-1)
                {
                    strList.append(str);
                    continue;
                }

                int nextIndex = i+1 < length ? i+1 : length-1;
                if(line[nextIndex]==',')//字段结束
                {
                    insideFlag = false;
                    quoteCount = 0;
                    i++;
                    strList.append(str);
                    str.clear();
                    continue;
                }
                else if(line[nextIndex]=='"')//字段内引号，应该成对出现
                {
                    quoteCount++;
                    continue;
                }
                else//格式错误，放弃此行
                {
                    return QStringList();
                }
            }

            str.append(line[i]);

        }
        else if(line[i] == ',' && !insideFlag)
        {
            strList.append(str);
            str.clear();
        }
        else
        {
            str.append(line[i]);
        }
        if(i == length-1)
        {
            strList.append(str);
            break;
        }

    }
    return qMove(strList);
}

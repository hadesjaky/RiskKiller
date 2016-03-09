#include "cfgmanager.h"
#include <QDir>
#include <QTextStream>
#include <QMessageBox>

CFGManager* CFGManager::_instance = 0;
CFGManager *CFGManager::Instance()
{
    if (_instance == 0)
    {
        _instance = new CFGManager();
    }
    return _instance;
}
QVector<CFGFile *> CFGManager::getCFGContainer() const
{
    return CFGContainer;
}

QMap<QString, QString> CFGManager::getRelationMap() const
{
    return relationMap;
}
CFGManager::CFGManager(QObject *parent) :
    QObject(parent)
{

}
void CFGManager::clearAll()
{
    for(int i=0;i<CFGContainer.size();++i)
        delete CFGContainer[i];
    CFGContainer.clear();
    relationMap.clear();
}
void CFGManager::loadCFGFile(QString path)
{
    clearAll();//先清空
    QFile cfgFile(path);
    if (!cfgFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //QMessageBox::warning(???, tr("警告："), tr("读取文件失败"));
        return;
    }

    QTextStream cfgStream(&cfgFile);
    QString line;
    CFGFile* currentCFG = NULL;

    while(!cfgStream.atEnd())
    {
        line = cfgStream.readLine();

        if(line.isEmpty() || line.size()==0 || "#"==line.at(0))
        {
            continue;
        }

        if(line == "[sfile]" || line == "[dfile]")//进入配置节,创建一个cfgfile对象
        {
            currentCFG  = new CFGFile();
            CFGContainer.append(currentCFG);
            if(line.contains("sfile"))
            {
                currentCFG->setType("S");//设置静态文件
            }
            else if(line.contains("dfile"))
            {
                currentCFG->setType("D");//设置动态文件
            }
        }
        else
        {
            QStringList cfgList = line.split("=");
            if(cfgList.size() != 2)
                continue;
            QString key = cfgList.at(0);
            QString value = cfgList.at(1);

            if(key == "separator")
            {
                currentCFG->setSep(value);
            }
            else if(key == "path")
            {
                currentCFG->setPath(QDir::currentPath() + "/" + value);//jaky，加个符号
            }
            else if(key == "quoet")
            {
                currentCFG->setQuoet(value);
            }
            else if(key == "资金关系")//增加关系的绑定，没有找到，则使用默认值
            {
                relationMap.insert(key,value);
            }
            else if(key == "设备关系")
            {
                relationMap.insert(key,value);
            }
            else if(key == "证件关系")
            {
                relationMap.insert(key,value);
            }
            else if(key == "社工关系")
            {
                relationMap.insert(key,value);
            }
            else if(key == "地址关系")
            {
                relationMap.insert(key,value);
            }
            else if(key == "IP关系")
            {
                relationMap.insert(key,value);
            }
            else if(key == "手机号关系")
            {
                relationMap.insert(key,value);
            }
            else if(key == "Mac地址关系")
            {
                relationMap.insert(key,value);
            }
            else if(key == "ATM号关系")
            {
                relationMap.insert(key,value);
            }
            else
            {
                currentCFG->addField(key, value.toInt());
            }
        }
    }
    cfgStream.flush();
    cfgFile.close();
}



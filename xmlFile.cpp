#include "xmlFile.h"
#include <QDebug>

XmlFile::XmlFile(QString _fileName): ReadFileBase(_fileName)
{
    fileName = _fileName;
    separator = ",";//默认分隔符为","
    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()), this, SLOT(checkProgress()));
    timer->start(200);
}

XmlFile::~XmlFile()
{

}
void XmlFile::loadData()
{
    TiXmlDocument doc(fileName.toStdString());
    doc.LoadFile(TIXML_ENCODING_UNKNOWN);

    if(doc.Error()&&doc.ErrorId() == TiXmlBase::TIXML_ERROR_OPENING_FILE )
    {
        return ;
    }

    TiXmlNode *node=doc.FirstChild("info");
    if(node){
        //info
        TiXmlElement *element=node->ToElement();

        for (TiXmlElement *secLayerElement=element->FirstChildElement();
             secLayerElement;secLayerElement=secLayerElement->NextSiblingElement()){
            //User
            LogicPerson *person = new LogicPerson();
            QString name = QString::fromStdString(secLayerElement->Attribute("name"));
            person->setName(name);
            QString id = QString::fromStdString(secLayerElement->Attribute("id"));
            person->setId(id);
            QString customerNo = QString::fromStdString(secLayerElement->Attribute("customerNO"));
            person->setCustomerNO(customerNo);
            QString cardNo = QString::fromStdString(secLayerElement->Attribute("cardNO"));
            person->setCardNO(cardNo.split(separator));
            QString mobile = QString::fromStdString(secLayerElement->Attribute("mobile"));
            person->setMobile(mobile.split(separator));
            QString level = QString::fromStdString(secLayerElement->Attribute("level"));
            person->pushLevel(level);
            QString email = QString::fromStdString(secLayerElement->Attribute("email"));
            person->addEmail(email);
            QString nebor = QString::fromStdString(secLayerElement->Attribute("nebor"));
            // qDebug() << name;
            //getUserNext(secLayerElement, person);
            People::Instance()->registerPerson(person);
        }
        for (TiXmlElement *childElement=element->FirstChildElement();
             childElement;childElement=childElement->NextSiblingElement()){

            QString customerNo = QString::fromStdString(childElement->Attribute("customerNO"));
            LogicPerson *masterCust = People::Instance()->getPerson(customerNo);
            getUserNext(childElement ,masterCust);


        }
    }
    connect(this, SIGNAL(loadFinished()), this, SLOT(stopTimer()));
    emit loadFinished();
}
void XmlFile::run()
{
    loadData();
}
void XmlFile::getUserNext(TiXmlElement *user, LogicPerson *p)
{
    for (TiXmlElement *paramType = user->FirstChildElement();
         paramType;paramType=paramType->NextSiblingElement())
    {
        if(paramType->ValueStr() == "son")
        {

            QString name = QString::fromStdString(paramType->Attribute("name"));
            QString id = QString::fromStdString(paramType->Attribute("id"));
            QString customerNo = QString::fromStdString(paramType->Attribute("customerNO"));
            QString cardNo = QString::fromStdString(paramType->Attribute("cardNO"));
            QString mobile = QString::fromStdString(paramType->Attribute("mobile"));
            QString level = QString::fromStdString(paramType->Attribute("level"));
            QString email = QString::fromStdString(paramType->Attribute("email"));
            QString linkType = QString::fromStdString(paramType->Attribute("linkType"));
            QString link = linkType.split(",").join("|");
            LogicPerson* slaveCust = People::Instance()->getPerson(cardNo);
            p->addSon(slaveCust,link);
            //qDebug()<<QString::fromStdString( paramType->Attribute("name"));
        }
        else if(paramType->ValueStr() == "father")
        {
            QString name = QString::fromStdString(paramType->Attribute("name"));
            QString id = QString::fromStdString(paramType->Attribute("id"));
            QString customerNo = QString::fromStdString(paramType->Attribute("customerNO"));
            QString cardNo = QString::fromStdString(paramType->Attribute("cardNO"));
            QString mobile = QString::fromStdString(paramType->Attribute("mobile"));
            QString level = QString::fromStdString(paramType->Attribute("level"));
            QString email = QString::fromStdString(paramType->Attribute("email"));
            QString linkType = QString::fromStdString(paramType->Attribute("linkType"));
            QString link = linkType.split(",").join("|");

            LogicPerson* slaveCust = People::Instance()->getPerson(cardNo);
            p->addFather(slaveCust,link);
        }
        else
            qDebug() << "only father or son! please check!";
    }
}
void XmlFile::getSons(TiXmlElement *son, LogicPerson *p)
{

}
void XmlFile::getFathers(TiXmlElement *father, LogicPerson *p)
{

}
void XmlFile::checkProgress()
{
    //emit updateProgress(tr("正在导入文件:%1,进度%2 %, 当前行数：%3").arg(curFileName).arg(1.0*curSize/totalSize*100).arg(lineNO));
}

void XmlFile::stopTimer()
{
    timer->stop();
}

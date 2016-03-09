#include "savecCache.h"
#include <QDebug>
#include <QFile>

saveCache::saveCache()
{

}
saveCache::saveCache(QMap<QString, ItemBase*> _itemContainer)
{
    m_itemContainer = _itemContainer;
}

saveCache::~saveCache()
{

}
void saveCache::fileSaveAsFile(QString name)
{
    TiXmlDocument outPutDoc;
    TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );
    outPutDoc.LinkEndChild( decl );
    TiXmlElement * root = new TiXmlElement( "info" );
    outPutDoc.LinkEndChild( root );
    QList<ItemBase*> itemList = m_itemContainer.values();

    TiXmlElement *user = NULL;
    TiXmlElement *son = NULL;
    TiXmlElement *father = NULL;
    for(int i=0;i <itemList.size(); ++i)
    {
        user = new TiXmlElement("user");
        root->LinkEndChild(user);
        voluation(user, itemList[i]," ", SELF);
        for(QMap<ItemBase*, QString>::const_iterator it=itemList[i]->getSon().constBegin(); it!= itemList[i]->getSon().constEnd();++it)
        {
            son = new TiXmlElement("son");
            user->LinkEndChild(son);
            voluation(son, it.key(),it.value(), SON);
        }
        for(QMap<ItemBase*, QString>::const_iterator itf=itemList[i]->getFather().constBegin(); itf!= itemList[i]->getFather().constEnd();++itf)
        {
            father = new TiXmlElement("father");
            user->LinkEndChild(father);
            voluation(father, itf.key(),itf.value(), FATHER);
        }
    }
    outPutDoc.SaveFile(name.toStdString());

}
void saveCache::voluation(TiXmlElement *e, ItemBase *_item, QString str, LINKTYPE t)
{
    e->SetAttribute("name" ,_item->getName().toStdString());
    e->SetAttribute("id" ,_item->getId().toStdString());
    e->SetAttribute("customerNO" ,_item->getCustomerNO().toStdString());
    e->SetAttribute("cardNO" ,_item->getCardNO().join(",").toStdString());
    e->SetAttribute("mobile" ,_item->getMobile().join(",").toStdString());
    e->SetAttribute("level" ,_item->getLevel().join(",").toStdString());
    if(t != SELF)
        e->SetAttribute("linkType" ,str.split("|").join(",").toStdString());
    e->SetAttribute("email" ,_item->getEmail().join(",").toStdString());
    if(t == SELF)
        e->SetAttribute("nebor" ,_item->getLinkerNum());
}

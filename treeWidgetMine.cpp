#include "treeWidgetMine.h"

TreeWidgetMine::TreeWidgetMine(QTreeWidget* _tree,QWidget *parent) : QWidget(parent)
{
    tree = _tree;
    init();
}

TreeWidgetMine::~TreeWidgetMine()
{

}
void TreeWidgetMine::init()
{
    QStringList s ;
    s.clear();
    s << tr("姓名");
    name = new QTreeWidgetItem(tree,s);

    s.clear();
    s<< tr("证件号");
    ID = new QTreeWidgetItem(tree, s);

    s.clear();
    s<< tr("客户编号");
    customerNo = new QTreeWidgetItem(tree, s);

    s.clear();
    s<< tr("邻居数");
    linkerNum = new QTreeWidgetItem(tree, s);

    s.clear();
    s<< tr("银行卡号");
    cardNo = new QTreeWidgetItem(tree, s);

    s.clear();
    s<< tr("手机号");
    mobile = new QTreeWidgetItem(tree, s);

    s.clear();
    s<< tr("最小层级");
    minLevel = new QTreeWidgetItem(tree, s);


}
void TreeWidgetMine::clearAll()
{
    if(listTreeItem.isEmpty())
        return;
    for(int i=0;i<listTreeItem.size();++i)
    {
        //tree->removeItemWidget(listTreeItem[i],0);
       // tree->removeItemWidget(listTreeItem[i], 1);
        delete listTreeItem[i];
    }
    listTreeItem.clear();
}
void TreeWidgetMine::nameOp(QStringList s)
{
    for(int i=0;i < s.size();++i)
    {
        if(i==0)
            name->setText(1,s[0]);
        else{
            QTreeWidgetItem *it = new QTreeWidgetItem(name);
            it->setText(1,s[i]);
            listTreeItem.append(it);
        }
    }
}
void TreeWidgetMine::IDOp(QStringList s)
{
    for(int i=0;i < s.size();++i)
    {
        if(i==0)
            ID->setText(1,s[0]);
        else{
            QTreeWidgetItem *it = new QTreeWidgetItem(ID);
            it->setText(1,s[i]);
            listTreeItem.append(it);
        }
    }
}
void TreeWidgetMine::customerNoOp(QStringList s)
{
    for(int i=0;i < s.size();++i)
    {
        if(i==0)
            customerNo->setText(1,s[0]);
        else{
            QTreeWidgetItem *it = new QTreeWidgetItem(customerNo);
            it->setText(1,s[i]);
            listTreeItem.append(it);
        }
    }
}
void TreeWidgetMine::linkerNumOp(QStringList s)
{
    for(int i=0;i < s.size();++i)
    {
        if(i==0)
            linkerNum->setText(1,s[0]);
        else{
            QTreeWidgetItem *it = new QTreeWidgetItem(linkerNum);
            it->setText(1,s[i]);
            listTreeItem.append(it);
        }
    }
}
void TreeWidgetMine::cardNoOp(QStringList s)
{
    for(int i=0;i < s.size();++i)
    {
        if(i==0)
            cardNo->setText(1,s[0]);
        else{
            QTreeWidgetItem *it = new QTreeWidgetItem(cardNo);
            it->setText(1,s[i]);
            listTreeItem.append(it);
        }
    }
}
void TreeWidgetMine::mobileOp(QStringList s)
{
    for(int i=0;i < s.size();++i)
    {
        if(i==0)
            mobile->setText(1,s[0]);
        else{
            QTreeWidgetItem *it = new QTreeWidgetItem(mobile);
            it->setText(1,s[i]);
            listTreeItem.append(it);
        }
    }
}
void TreeWidgetMine::minLevelOp(QStringList s)
{
    for(int i=0;i < s.size();++i)
    {
        if(i==0)
            minLevel->setText(1,s[0]);
        else{
            QTreeWidgetItem *it = new QTreeWidgetItem(minLevel);
            it->setText(1,s[i]);
            listTreeItem.append(it);
        }
    }
}

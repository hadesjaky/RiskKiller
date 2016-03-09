#ifndef TREEWIDGETMINE_H
#define TREEWIDGETMINE_H

#include <QWidget>
#include <QTreeWidget>
/*
两列，第一列属性，第二列是值
*/

class TreeWidgetMine : public QWidget
{
    Q_OBJECT
public:
    explicit TreeWidgetMine(QTreeWidget* _tree, QWidget *parent = 0);
    ~TreeWidgetMine();
    void init();
    void clearAll(); //清空操作
    void nameOp(QStringList s);
    void IDOp(QStringList s);
    void customerNoOp(QStringList s);
    void linkerNumOp(QStringList s);
    void cardNoOp(QStringList s);
    void mobileOp(QStringList s);
    void minLevelOp(QStringList s);
signals:

public slots:
private:
    QTreeWidget * tree;
    QTreeWidgetItem *name;
    QTreeWidgetItem *ID;
    QTreeWidgetItem *customerNo;
    QTreeWidgetItem *linkerNum;
    QTreeWidgetItem *cardNo;
    QTreeWidgetItem *mobile;
    QTreeWidgetItem *minLevel;
    QList<QTreeWidgetItem*> listTreeItem;

};

#endif // TREEWIDGETMINE_H

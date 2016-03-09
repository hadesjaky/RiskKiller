#ifndef EXCEPTMENU_H
#define EXCEPTMENU_H

#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QGraphicsItem>
#include "itembase.h"
/*
结点属性栏内的例外菜单,鼠标右键出现重复的菜单
*/


class ExceptMenu : public QWidget
{
    Q_OBJECT
public:
    ~ExceptMenu();
    static ExceptMenu *exceptIns();
    QMenu* getExceptMenu();
    QAction* getOtherBank();
    QAction* getThirdPart();

    QMenu* getItemMenu(QList<QAction *> list, QMenu* m);
signals:

public slots:
private:
    explicit ExceptMenu(QWidget *parent = 0);
    static ExceptMenu *except;
    QMenu *m_menu;
    QAction *m_otherBank;
    QAction *m_ThirdPart;
    QMenu *m_itemMenu;

};

#endif // EXCEPTMENU_H

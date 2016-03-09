#include "exceptMenu.h"
ExceptMenu* ExceptMenu::except;

ExceptMenu::ExceptMenu(QWidget *parent) : QWidget(parent)
{
    m_menu = new QMenu();
    m_otherBank = new QAction(m_menu);
    m_ThirdPart = new QAction(m_menu);
    m_otherBank->setCheckable(true);
    m_otherBank->setText("其他银行");
    m_ThirdPart->setCheckable(true);
    m_ThirdPart->setText("第三方支付");
    m_menu->addAction(m_otherBank);
    m_menu->addAction(m_ThirdPart);

    m_itemMenu = new QMenu();
}

ExceptMenu::~ExceptMenu()
{

}
ExceptMenu *ExceptMenu::exceptIns()
{
    if(except == NULL)
        except = new ExceptMenu();
    return except;
}
QMenu* ExceptMenu::getExceptMenu()
{
    return m_menu;
}
QAction* ExceptMenu::getOtherBank()
{
    return m_otherBank;
}
QAction* ExceptMenu::getThirdPart()
{
    return m_ThirdPart;
}

QMenu* ExceptMenu::getItemMenu(QList<QAction *> list, QMenu* m)
{
    for(int i=0; i<list.size();++i)
        m->addAction(list[i]);
    return m;
}

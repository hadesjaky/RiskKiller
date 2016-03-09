#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qmainwindow.h"
#include "qabstractbutton.h"
#include "scene.h"
#include "globaldef.h"
#include "itembase.h"
#include <QDoubleSpinBox>
#include <QToolButton>
#include <QTreeWidgetItem>
#include "colorEditer/colorPlanBoard.h"
#include "statMmanager.h"
#include "treeWidgetMine.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void routePointSIG(QString str);
    void nodefilterSIG(QString, bool flag);
private:
    void initButtonName();
    void createToolBar();
    void crtUIActConnect();//创建界面动作的连接
    Ui::MainWindow *ui;
    Scene *scene;
    QDoubleSpinBox *scalingSpinBox;
    QDoubleSpinBox *rotationSpinBox;
    QToolButton *selectBtn;
    QToolButton *dragBtn;
    TreeWidgetMine *m_tree;

    ColorPlanBoard *cpBoard;//调色板

    void setObName(QMap<QString,QString> m);


private slots:
    void loadData();
    void loadTestData();
    void changeDrawMethod(QAbstractButton *);
    void changShowFlag(QAbstractButton*, bool);//改变checkbox状态
    //void changeRelationFlag(int, bool);//改变<关系选择框>的状态
    void initCheckBox();
    void showItemByLinkerNum(int);//显示临结点数
    void setTopLevelColor(bool flag);//首层作色
    void setFontSize(int);//调整结点大小
    void setNodeInterval(int value);//调整结点间距
    void showBarMsg(QString msg);
    void initView();//视图重置
    void showInfo(ItemBase *item);//jaky,treeWidget内容显示
    void fixedPoint();//查找定点
    void modeSelect();//模式选择
   // void findNode();//查找结点
    void processAction();//处理动作
    void processBoolAction(bool b);

};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "itembase.h"
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QIcon>
#include <QToolButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "cfgmanager.h"
#include <QDoubleSpinBox>
#include <QDialog>
#include <QColorDialog>
#include <QDebug>
#include "findNodeDialog.h"
#include "savecCache.h"
#include "xmlFile.h"
#include "readFileBase.h"
#include "datareader.h"
#include "exceptMenu.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_tree = new TreeWidgetMine(ui->treeWidget);
    initButtonName();
    createToolBar();
    crtUIActConnect();
    scene = Scene::Instance();
    ui->view->setScene(scene);

    //显示关系控制栏
    //结点属性显示控制栏
    connect(ui->attrButtonGroup, SIGNAL(buttonToggled(QAbstractButton*, bool)), this, SLOT(changShowFlag(QAbstractButton*, bool)));
    connect(this,SIGNAL(nodefilterSIG(QString,bool)),scene,SLOT(nodeFilter(QString,bool)));
    //connect(ui->attrButtonGroup,SIGNAL(buttonToggled(int,bool)),this,SLOT(changeRelationFlag(int,bool)));
    ui->moneyRelationBox->setChecked(true);
    ui->idRelationBox->setChecked(true);
    ui->addrBox->setChecked(true);
    ui->societyRealtionBox->setChecked(true);
    ui->IPBox->setChecked(true);
    ui->phoneBox->setChecked(true);
    ui->MacBox->setChecked(true);
    ui->ATMBox->setChecked(true);
    //分析控制栏
    connect(ui->someNetBox, SIGNAL(clicked(bool)), ui->view, SLOT(drawNetBySomeone(bool)));
    connect(ui->linkerNumSpinBox, SIGNAL(valueChanged(int)), this, SLOT(showItemByLinkerNum(int)));
    connect(ui->topLevelColorBox, SIGNAL(clicked(bool)), this, SLOT(setTopLevelColor(bool)));
    connect(ui->fontSizeBox, SIGNAL(valueChanged(int)), this, SLOT(setFontSize(int)));
    connect(ui->findNodeBtn,SIGNAL(clicked()),this,SLOT(fixedPoint()));//定点寻址
    connect(this,SIGNAL(routePointSIG(QString )),ui->view, SLOT(routePoint(QString)));
    //jaky,
    connect(ui->view,SIGNAL(showTreeWgt(ItemBase*)),this,SLOT(showInfo(ItemBase*)));
    connect(ui->nodeDisBOx, SIGNAL(valueChanged(int)), this,SLOT(setNodeInterval(int)));

    //状态栏信息更新
    connect(scene, SIGNAL(sendMsg(QString)), this, SLOT(showBarMsg(QString)));
    connect(ui->view, SIGNAL(sendMsg(QString)), this, SLOT(showBarMsg(QString)));
    setWindowTitle("RiskKiller");
    //ui->page_1->setStyleSheet("font:75 9pt Aharoni;");
    ui->toolBox->setStyleSheet("QToolBox::tab:selected{font:12pt; color: rgb(2, 2, 124);}");

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::setObName(QMap<QString,QString> m)
{
    QMap<QString,QString>::const_iterator it = m.constBegin();
    while(it != m.constEnd())
    {
        if(it.key() == "资金关系")
            ui->moneyRelationBox->setObjectName(it.value());
        else if(it.key() == "证件关系")
            ui->idRelationBox->setObjectName(it.value());
        else if(it.key() == "社工关系")
            ui->societyRealtionBox->setObjectName(it.value());
        else if(it.key() == "地址关系")
            ui->addrBox->setObjectName(it.value());
        else if(it.key() == "IP关系")
            ui->IPBox->setObjectName(it.value());
        else if(it.key() == "手机号关系")
            ui->phoneBox->setObjectName(it.value());
        else if(it.key() == "Mac地址关系")
            ui->MacBox->setObjectName(it.value());
        else if(it.key() == "ATM号关系")
            ui->ATMBox->setObjectName(it.value());
        it++;
    }
}
void MainWindow::loadData()
{
    QFileDialog *fd = new QFileDialog(this, tr("请选择导入文件"), QDir::currentPath()+"/config","*.ini *.xml");
    fd->setFileMode(QFileDialog::AnyFile);
    fd->setViewMode(QFileDialog::Detail);
    QStringList fileNamesList;
    if(fd->exec()) // ok
    {
        fileNamesList=fd->selectedFiles();
    }
    else
    {
        QMessageBox::warning(this, tr("警告"), tr("获取数据文件失败") );
        return ;
    }
    ui->view->zoom(1.0);
    scalingSpinBox->setValue(1.0);
    QString fileName = fileNamesList.at(0).toLocal8Bit().constData();
    QStringList fileType = fileName.split(".");
    if(fileType.last() == "xml")
    {
        ReadFileBase *file = new XmlFile(fileName);
        scene->loadData(file);
    }
    else if(fileType.last() == "ini")
    {
        ReadFileBase *file = new DataReader(fileName);
        CFGManager::Instance()->loadCFGFile(fileName);//读取静态文件
        this->setObName(CFGManager::Instance()->getRelationMap());
        scene->loadData(file);
    }
    else
        return;
}

void MainWindow::loadTestData()
{
    ui->view->zoom(1.0);
    scalingSpinBox->setValue(1.0);
    CFGManager::Instance()->loadCFGFile( QDir::currentPath()+ "\\config\\dataconfig-all.ini");
    //scene->loadData(QDir::currentPath()+ "\\config\\dataconfig.ini");
}

void MainWindow::changeDrawMethod(QAbstractButton *b)
{
    Scene::Instance()->drawPaths(b->objectName().toInt());
}

void MainWindow::changShowFlag(QAbstractButton *button, bool flag)
{
    QString  objectName = button->objectName();
    //qDebug() << objectName;
    ItemBase::changeShowFlag(objectName, flag);
    StateManager::stateIns()->showFlagsMan.insert(objectName, flag);
    emit nodefilterSIG(objectName, flag);
    ui->view->viewport()->update();
}

void MainWindow::initCheckBox()
{
    disconnect(ui->someNetBox, SIGNAL(clicked(bool)), ui->view, SLOT(drawNetBySomeone(bool)));
    ui->someNetBox->setCheckState(Qt::Unchecked);
    connect(ui->someNetBox, SIGNAL(clicked(bool)), ui->view, SLOT(drawNetBySomeone(bool)));
}

void MainWindow::showItemByLinkerNum(int num)
{
    ItemBase::setShowLinkerNum(num);
    ui->view->viewport()->update();
}

void MainWindow::setTopLevelColor(bool flag)
{
    ItemBase::setTopLevelColor(flag);
    ui->view->viewport()->update();
}

void MainWindow::setFontSize(int size)
{
    ItemBase::setFontSize(size);
    ui->view->viewport()->update();
}
void MainWindow::setNodeInterval(int value)
{
    Scene::Instance()->setNodeInterval(value);
    ui->view->viewport()->update();
}
void MainWindow::showBarMsg(QString msg)
{
    if(scene->getItemContainer().size() > 0)
    {
        msg = tr("客户总数：%1人\t").arg(scene->getItemContainer().size()) + msg;
    }

    ui->statusBar->showMessage(msg);

}

void MainWindow::initView()
{//角度，放大回零，视图清空重新回初始型
    ui->view->rotateAngle(0.0);
    ui->view->zoom(1.0);
    scene->init();
    ui->view->showAll();
}


void MainWindow::createToolBar()
{
    QAction *loadAct = ui->mainToolBar->addAction(tr("导入"));
    loadAct->setObjectName("loadAct");
    loadAct->setIcon(QIcon(":/res/open.png"));
    loadAct->setIconText(tr("导入"));
    connect(loadAct, SIGNAL(triggered()), this, SLOT(processAction()));
    //模式切换栏
    QToolBar *modeSelect = new QToolBar(tr("模式选择栏"), this);
    QButtonGroup *modeGroup = new QButtonGroup(this);
    modeGroup->setExclusive(true);

    selectBtn = new QToolButton;
    selectBtn->setText(tr("选择"));
    selectBtn->setCheckable(true);
    selectBtn->setChecked(true);
    dragBtn = new QToolButton;
    dragBtn->setText(tr("拖拽"));
    dragBtn->setCheckable(true);
    dragBtn->setChecked(false);
    modeSelect->addWidget(selectBtn);
    modeSelect->addWidget(dragBtn);
    modeGroup->addButton(selectBtn);
    modeGroup->addButton(dragBtn);
    modeSelect->addSeparator();
    addToolBar(modeSelect);
    connect(selectBtn,SIGNAL(toggled(bool)),this,SLOT(modeSelect()));
    connect(dragBtn,SIGNAL(toggled(bool)),this,SLOT(modeSelect()));
    //快捷工具栏创建
    QButtonGroup *group = new QButtonGroup(this);
    group->setExclusive(true);

    QToolBar *shortcutCtrl = new QToolBar(tr("快捷工具栏"), this);
    QToolButton *levelAct = new QToolButton(this);
    //levelAct->setIcon(QIcon(":/res/refresh.png"));
    levelAct->setText(tr("分层"));
    //levelAct->setCheckable(true);
    levelAct->setEnabled(true);
    levelAct->setObjectName(QString("%1").arg(LEVEL));
    shortcutCtrl->addWidget(levelAct);
    group->addButton(levelAct);

    QToolButton *cycleAct = new QToolButton(this);
    //levelAct->setIcon(QIcon(":/res/refresh.png"));
    cycleAct->setText(tr("画圈"));
    //cycleAct->setCheckable(true);
    cycleAct->setEnabled(true);
    cycleAct->setObjectName(QString("%1").arg(CYCLE));
    shortcutCtrl->addWidget(cycleAct);
    group->addButton(cycleAct);

    QToolButton *show3Act = new QToolButton(this);
    //levelAct->setIcon(QIcon(":/res/refresh.png"));
    show3Act->setText(tr("算法三"));
    //show3Act->setCheckable(true);
    show3Act->setEnabled(true);
    show3Act->setObjectName(QString("%1").arg(FINDPATHS));
    shortcutCtrl->addWidget(show3Act);
    group->addButton(show3Act);

    QToolButton *show4Act = new QToolButton(this);
    //levelAct->setIcon(QIcon(":/res/refresh.png"));
    show4Act->setText(tr("算法四"));
    //show4Act->setCheckable(true);
    show4Act->setEnabled(true);
    show4Act->setObjectName(QString("%1").arg(M4));
    shortcutCtrl->addWidget(show4Act);
    group->addButton(show4Act);

    shortcutCtrl->addSeparator();
    addToolBar(shortcutCtrl);
    connect(group, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(changeDrawMethod(QAbstractButton *)));

    //显示控制栏
    QToolBar *viewCtrlBar = new QToolBar(tr("视图控制栏"),this);
    QAction *zoomInAct = new QAction("放大", this);
    zoomInAct->setIcon(QIcon(":/res/zoomin.png"));
    zoomInAct->setIconText(tr("放大"));
    viewCtrlBar->addAction(zoomInAct);
    addToolBar(viewCtrlBar);
    connect(zoomInAct, SIGNAL(triggered()), ui->view, SLOT(zoomIn()));

    QAction *zoomOutAct = new QAction("缩小", this);
    zoomOutAct->setIcon(QIcon(":/res/zoomout.png"));
    zoomOutAct->setIconText(tr("缩小"));
    viewCtrlBar->addAction(zoomOutAct);
    //addToolBar(viewCtrlBar);
    connect(zoomOutAct, SIGNAL(triggered()), ui->view, SLOT(zoomOut()));

    scalingSpinBox = new QDoubleSpinBox(this);
    scalingSpinBox->setMinimum(0.01);
    scalingSpinBox->setSingleStep(0.01);
    scalingSpinBox->setValue(1.0);
    //scalingSpinBox->setDecimals(8);

    QLabel *scalingLabel = new QLabel(tr("缩放比:"), this);
    viewCtrlBar->addWidget(scalingLabel);
    viewCtrlBar->addWidget(scalingSpinBox);
    viewCtrlBar->addSeparator();
    connect(scalingSpinBox, SIGNAL(valueChanged(double)), ui->view, SLOT(zoom(qreal)));
    connect(ui->view, SIGNAL(scalingChange(qreal)), scalingSpinBox, SLOT(setValue(double)));

    //旋转
    QAction *clockwiseAct = new QAction("顺时针旋转", this);
    clockwiseAct->setIcon(QIcon(":/res/clockwise.png"));
    clockwiseAct->setIconText(tr("顺时针旋转"));
    viewCtrlBar->addAction(clockwiseAct);
    connect(clockwiseAct, SIGNAL(triggered()), ui->view, SLOT(rotateClockwise()));

    QAction *antiClockwiseAct = new QAction("逆时针旋转", this);
    antiClockwiseAct->setIcon(QIcon(":/res/anticlockwise.png"));
    antiClockwiseAct->setIconText(tr("逆时针旋转"));
    viewCtrlBar->addAction(antiClockwiseAct);
    connect(antiClockwiseAct, SIGNAL(triggered()), ui->view, SLOT(rotateAntiClockwise()));

    rotationSpinBox = new QDoubleSpinBox(this);
    rotationSpinBox->setMinimum(-360.0);
    rotationSpinBox->setMaximum(360.0);
    rotationSpinBox->setSingleStep(1.0);
    rotationSpinBox->setValue(0.0);

    QLabel *rotationLabel = new QLabel(tr("旋转度:"), this);
    viewCtrlBar->addWidget(rotationLabel);
    viewCtrlBar->addWidget(rotationSpinBox);
    connect(rotationSpinBox, SIGNAL(valueChanged(double)), ui->view, SLOT(rotateAngle(qreal)));
    connect(ui->view, SIGNAL(rotationChange(qreal)), rotationSpinBox, SLOT(setValue(double)));
    viewCtrlBar->addSeparator();

    QAction *resetAct = new QAction("视图重置", this);
    resetAct->setIcon(QIcon(":/res/refresh.png"));
    resetAct->setIconText(tr("视图重置"));
    viewCtrlBar->addAction(resetAct);
    connect(resetAct, SIGNAL(triggered()), this, SLOT(initView()));

    //仅供测试使用
    QToolBar *debugBar = new QToolBar(tr("测试工具栏"),this);
    QAction *debugAct = new QAction("测试", this);
    debugAct->setIcon(QIcon(":/res/debug.png"));
    debugAct->setIconText(tr("导入"));
    debugBar->addAction(debugAct);
    addToolBar(debugBar);
    connect(debugAct, SIGNAL(triggered()), this, SLOT(loadTestData()));
}
void MainWindow::crtUIActConnect()
{
    connect(ui->openAct, SIGNAL(triggered()),this,SLOT(processAction()));//打开
    connect(ui->findAct,SIGNAL(triggered()),this, SLOT(processAction()));//查找
    connect(ui->saveAsAct, SIGNAL(triggered()),this,SLOT(processAction()));//另存为
    connect(ui->saveAct,SIGNAL(triggered()),this, SLOT(processAction()));//保存


    connect(ui->delAct, SIGNAL(triggered(bool)),this,SLOT(processBoolAction(bool)));//删除
    connect(ui->YCAct, SIGNAL(triggered(bool)),this,SLOT(processBoolAction(bool)));//隐藏
    connect(ui->palletAct, SIGNAL(triggered(bool)), this, SLOT(processBoolAction(bool)));//调色板
    connect(ui->mergeAct, SIGNAL(triggered(bool)), this, SLOT(processBoolAction(bool)));//调色板
}
void MainWindow::initButtonName()
{
    ui->editFindNode->setPlaceholderText("请输入结点编号");
    //结点属性框
    ui->hideLineBox->setObjectName(tr("%1").arg(HIDE_PATH_A));
    ui->nameBox->setObjectName(tr("%1").arg(NAME_A));
    ui->idBox->setObjectName(tr("%1").arg(ID_A));
    ui->customerNOBox->setObjectName(tr("%1").arg(CUSTOMER_NO_A));
    ui->levelBox->setObjectName(tr("%1").arg(LEVEL_A));
    ui->moblieBox->setObjectName(tr("%1").arg(MOBILE_A));
    ui->linkerNumBox->setObjectName(tr("%1").arg(LINKER_NUM_A));
    ui->emailBox->setObjectName(tr("%1").arg(EMAIL_A));
    ui->bankAddrBox->setObjectName(tr("%1").arg(BANK_ADDR_A));
    ui->cardNoBox->setObjectName(tr("%1").arg(CARD_NO_A));
    //ui->exceptBtn->setObjectName(tr("%1").arg("except"));//例外
    ui->exceptBtn->setMenu(ExceptMenu::exceptIns()->getExceptMenu());
    ExceptMenu::exceptIns()->getOtherBank()->setObjectName(tr("%1").arg(OTHER_BANK_A));
    ExceptMenu::exceptIns()->getThirdPart()->setObjectName(tr("%2").arg(THIRD_PART_A));
    //    ui->attrButtonGroup->addButton(ui->exceptBtn);
    //界面拖进group里面不能触发信号，需要从新加入组
    //    ui->attrButtonGroup->addButton(ui->cardNoBox);
    //    ui->attrButtonGroup->addButton(ui->addrBox);
    //    ui->attrButtonGroup->addButton(ui->IPBox);
    //    ui->attrButtonGroup->addButton(ui->moblieBox);
    //    ui->attrButtonGroup->addButton(ui->MacBox);
    //    ui->attrButtonGroup->addButton(ui->ATMBox);
    //    ui->attrButtonGroup->addButton(ui->exceptBtn);
    //    ui->attrButtonGroup->addButton(ui->phoneBox);

    ui->attrButtonGroup->addButton(ui->moneyRelationBox,1);
    ui->attrButtonGroup->addButton(ui->idRelationBox,2);
    ui->attrButtonGroup->addButton(ui->addrBox,3);
    ui->attrButtonGroup->addButton(ui->societyRealtionBox,4);
    ui->attrButtonGroup->addButton(ui->IPBox,5);
    ui->attrButtonGroup->addButton(ui->phoneBox,6);
    ui->attrButtonGroup->addButton(ui->MacBox,7);
    ui->attrButtonGroup->addButton(ui->ATMBox,8);

    //关系选择框
    //ui->devRealtionBox->setObjectName(tr("MOBILE"));     //设备关系
    ui->moneyRelationBox->setObjectName(tr("money"));    //资金关系
    ui->idRelationBox->setObjectName(tr("CARDNO"));      //证件关系
    ui->addrBox->setObjectName(tr("ADDRESS"));
    ui->societyRealtionBox->setObjectName(tr("society")); //社工关系
    ui->IPBox->setObjectName(tr("IP"));
    ui->phoneBox->setObjectName(tr("MOBILE"));
    ui->MacBox->setObjectName(tr("MAC"));
    ui->ATMBox->setObjectName(tr("ATM"));
}
void MainWindow::showInfo(ItemBase *item)
{
    m_tree->clearAll();
    m_tree->nameOp(item->getName().split(","));
    m_tree->IDOp(item->getId().split(","));
    m_tree->customerNoOp(item->getCustomerNO().split(","));
    m_tree->linkerNumOp(QString::number(item->getLinkerNum()).split(","));
    m_tree->cardNoOp(item->getCardNO());
    m_tree->mobileOp(item->getMobile());
    m_tree->minLevelOp(item->getMinLevel().split(","));
}
void MainWindow::fixedPoint()
{
    emit routePointSIG(ui->editFindNode->text());
}
void MainWindow::modeSelect()
{
    if(selectBtn->isChecked())
        StateManager::stateIns()->selectModel = true;
    else
        StateManager::stateIns()->selectModel = false;
    ui->view->setDragMode(selectBtn->isChecked()
                          ? QGraphicsView::RubberBandDrag
                          : QGraphicsView::ScrollHandDrag);
    ui->view->setInteractive(selectBtn->isChecked());
}

void MainWindow::processAction()
{
    if(sender()->objectName() == ui->openAct->objectName() || sender()->objectName() == "loadAct")//打开
    {
        scene->clearAll();
        this->loadData();
    }
    else if(sender()->objectName() == ui->saveAsAct->objectName() )//另存为
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                        "fileName.xml",
                                                        tr("结构关系xml文件 (*.xml)"));
        saveCache *ss = new saveCache(scene->getItemContainer());
        ss->fileSaveAsFile(fileName);
        delete ss;

    }
    else if(sender()->objectName() == ui->saveAct->objectName() )//保存
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                        "fileName.xml",
                                                        tr("结构关系xml文件 (*.xml)"));
        saveCache *ss = new saveCache(scene->getItemContainer());
        ss->fileSaveAsFile(fileName);
        delete ss;
    }

    else if(sender()->objectName() == ui->findAct->objectName() )//查找
    {
        FindNodeDialog d;
        connect(&d,SIGNAL(findStrSIG(QString)),ui->view,SLOT(findNode(QString)));
        d.exec();
    }
    else
        qDebug() << "no define";
}
void MainWindow::processBoolAction(bool b)
{
    if(sender()->objectName() == ui->delAct->objectName() )//删除
        StateManager::stateIns()->delNodeAct = b;
    else if(sender()->objectName() == ui->YCAct->objectName() )//隐藏
        StateManager::stateIns()->hiddenNodeAct = b;
    else if(sender()->objectName() == ui->palletAct->objectName() )//调色板
    {
        StateManager::stateIns()->colorBoard = b;
        if(b){
            //const QColor newColor = QColorDialog::getColor();
            cpBoard = new ColorPlanBoard();
            connect(cpBoard,SIGNAL(SIG_changeColor(QColor)),ui->view,SLOT(setColorNode(QColor)));
            cpBoard->show();
        }
        else
            delete cpBoard;
    }
    else if(sender()->objectName() == ui->mergeAct->objectName() )//合并
        StateManager::stateIns()->mergeNodeAct = b;
}

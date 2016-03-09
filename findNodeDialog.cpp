#include "findNodeDialog.h"
#include "ui_findNodeDialog.h"
#include <QDebug>

FindNodeDialog::FindNodeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindNodeDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    connect(ui->buttonBox,SIGNAL(accepted()),this, SLOT(processOkBtn()));
}

FindNodeDialog::~FindNodeDialog()
{
    delete ui;
}
void FindNodeDialog::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Return)//回车事件
    {
        emit findStrSIG(ui->lineEdit->text());
        this->accept();
    }
    else
    {
        QDialog::keyPressEvent(e);
    }
}
void FindNodeDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() == Qt::LeftButton) //当满足鼠标左键被点击时。
    {
        move(e->globalPos() - dragPosition);//移动窗口
    }
    else
        QDialog::mouseMoveEvent(e);
}
void FindNodeDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) //点击左边鼠标
    {
        dragPosition = e->globalPos() - frameGeometry().topLeft();
    }
    else
        QDialog::mouseMoveEvent(e);
}
void FindNodeDialog::processOkBtn()
{
    emit findStrSIG(ui->lineEdit->text());
}

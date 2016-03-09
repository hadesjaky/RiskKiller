#ifndef FINDNODEDIALOG_H
#define FINDNODEDIALOG_H

#include <QDialog>
#include <QKeyEvent>
#include <QMouseEvent>
/*
功能：查找结点对话框
*/

namespace Ui {
class FindNodeDialog;
}

class FindNodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindNodeDialog(QWidget *parent = 0);
    ~FindNodeDialog();
signals:
    void findStrSIG(QString str);
protected:
    void keyPressEvent(QKeyEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
private:
    Ui::FindNodeDialog *ui;
    QPoint dragPosition;
public slots:
    void processOkBtn();

};

#endif // FINDNODEDIALOG_H

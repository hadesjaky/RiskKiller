#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <QCheckBox>
class CheckBox : public QCheckBox
{
public:
    CheckBox(QWidget* parent=0);
    ~CheckBox();

    QString getAlias() const;
    void setAlias(const QString &value);

private:
    QString alias;

};

#endif // CHECKBOX_H

#ifndef CFGFILE_H
#define CFGFILE_H
#include <QString>
#include <QMap>
#include <QDebug>
#include <stdio.h>
#include <iostream>
//读取ini文件，保存 的
using namespace std;

class CFGFile
{
public:
    CFGFile();

    QString getSep() const;
    void setSep(const QString &value);

    QString getQuoet() const;
    void setQuoet(const QString &value);

    QString getPath() const;
    void setPath(const QString &value);

    QMap<QString, int> getFieldMap() const;
    void addField(const QString &fieldName, const int &pos);

    QString getType() const;
    void setType(const QString &value);
    //jaky，打印方法
    void printCFGFile();

private:
    QString sep;
    QString quoet;
    QString path;
    QMap<QString, int> fieldMap;
    QString type;
};


#endif // CFGFILE_H

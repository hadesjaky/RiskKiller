#ifndef CFGMANAGER_H
#define CFGMANAGER_H

#include <QObject>
#include <QString>
#include <QMap>
#include "cfgfile.h"
#include <QVector>

class CFGManager : public QObject
{
    Q_OBJECT
public:
    static CFGManager *Instance();
private:
    QVector<CFGFile*> CFGContainer;
    explicit CFGManager(QObject *parent = 0);
    static CFGManager* _instance;
    void clearAll();
    QMap<QString, QString> relationMap;//关系选择框的关系配置
public:
    void loadCFGFile(QString path);//装载*.ini文件

    QVector<CFGFile *> getCFGContainer() const;
    QMap<QString, QString> getRelationMap() const;

    //void setCFGContainer(const QVector<CFGFile *> &value);
};

#endif // CFGMANAGER_H

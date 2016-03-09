#ifndef XMLFILE_H
#define XMLFILE_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include "logicperson.h"
#include "tinyXml/tinyxml.h"
#include "tinyXml/tinystr.h"
#include "readFileBase.h"

class XmlFile : public QThread, public ReadFileBase
{
    Q_OBJECT

public:
    XmlFile(QString _fileName);
    ~XmlFile();
private:
    QString fileName;
    QString separator;
    void getUserNext(TiXmlElement *user, LogicPerson *p);
    void getSons(TiXmlElement *son, LogicPerson *p);
    void getFathers(TiXmlElement *father, LogicPerson *p);

    QTimer *timer;
signals:
    void loadFinished();
    void updateProgress(QString);
public slots:
    void loadData();
    void run();
    void checkProgress();
    void stopTimer();
};

#endif // XMLFILE_H

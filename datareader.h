#ifndef DATAREADER_H
#define DATAREADER_H

#include <QFile>
#include <QTextStream>
#include "cfgmanager.h"
#include "logicperson.h"
#include "cfgfile.h"
#include "globaldef.h"
#include <QVector>
#include <QThread>
#include <QTimer>
#include "readFileBase.h"

class DataReader : public QThread, public ReadFileBase
{
    Q_OBJECT

public:
    explicit DataReader(QString _fileName);
    ~DataReader();

private:
    QString separator;
    QMap<QString, int> fieldCFG;
    QStringList splitCSVLine(const QString &line);
    int totalSize;
    int curSize;
    QString curFileName;
    QTimer *timer;

    int lineNO;


signals:
    void loadFinished();
    void updateProgress(QString);
public slots:
    void loadData();
    void loadSData(CFGFile*cfg);
    void loadDData(CFGFile*cfg);
    void run();
    void checkProgress();
    void stopTimer();
};

#endif // DATAREADER_H

#ifndef READFILEBASE_H
#define READFILEBASE_H

#include <QObject>
#include <QString>

class ReadFileBase
{
public:
    ReadFileBase( QString _fileName);
    ~ReadFileBase();
    virtual void loadData() = 0;

    QString fileName;
};

#endif // READFILEBASE_H

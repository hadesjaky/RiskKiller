#include "cfgfile.h"

CFGFile::CFGFile()
{

}

QString CFGFile::getSep() const
{
    return sep;
}

void CFGFile::setSep(const QString &value)
{
    sep = value;
}
QString CFGFile::getQuoet() const
{
    return quoet;
}

void CFGFile::setQuoet(const QString &value)
{
    quoet = value;
}
QString CFGFile::getPath() const
{
    return path;
}

void CFGFile::setPath(const QString &value)
{
    path = value;
}
QMap<QString, int> CFGFile::getFieldMap() const
{
    return fieldMap;
}

void CFGFile::addField(const QString &fieldName, const int &pos)
{
    fieldMap.insert(fieldName, pos);
}
QString CFGFile::getType() const
{
    return type;
}

void CFGFile::setType(const QString &value)
{
    type = value;
}
void CFGFile::printCFGFile()
{
    cout << "separator : " << sep.toStdString().data() << "\n";
    cout << "quoet : " << quoet.toStdString().data() << "\n";
    cout << "path : " << path.toStdString().data() << "\n";
    cout << "type : " << type.toStdString().data() << "\n";
    //cout << "filedMap count : " << fieldMap.count() << "\n";

    //QMap<QString, int>::iterator it = fieldMap.begin();
    for(QMap<QString, int>::iterator it = fieldMap.begin();it != fieldMap.end(); ++it)
    {
        cout<<it.key().toStdString().data()<< "="<<it.value() << "\n";
    }
    cout<< "-------------------------------------"<<endl;
}

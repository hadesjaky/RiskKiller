#ifndef SAVECACHE_H
#define SAVECACHE_H
#include <QString>
#include "logicperson.h"
#include "itembase.h"
#include "./tinyXml/tinyxml.h"
#include "./tinyXml/tinystr.h"
/*功能：文件结构缓存,注:user标签没有linkType属性有nebor属性；son，father没有nebor属性，有linkType属性
<info>
  <user name="陈萌" id="010-116321" customerNO="ECIF0001" cardNO="62170021" mobile="10116,010-82808028" level="1" email="chenmeng@dist.com.cn" nebor="2">
    <son name="贾新格" id="0282" customerNO="ECIF0002" cardNO="62170022" mobile="13831193762,0311-88030066" level="1" linkType="" email="info@shineway.com"/>
    <father name="贾新格" id="0282" customerNO="ECIF0002" cardNO="62170022" mobile="13831193762,0311-88030066" level="1" linkType="" email="info@shineway.com"/>
  </user>
  <user name="贾新格" id="0282" customerNO="ECIF0002" cardNO="62170022" mobile="13831193762,0311-88030066" level="1" email="info@shineway.com" nebor="2">
    <son name="陈萌" id="010-116321" customerNO="ECIF0001" cardNO="62170021" mobile="10116,010-82808028" level="1" linkType="" email="chenmeng@dist.com.cn"/>
    <father name="陈萌" id="010-116321" customerNO="ECIF0001" cardNO="62170021" mobile="10116,010-82808028" level="1" linkType="" email="chenmeng@dist.com.cn"/>
  </user>
</info>
*/
enum LINKTYPE{SELF,SON,FATHER};
class saveCache
{
public:
    saveCache();
    saveCache(QMap<QString, ItemBase*> _itemContainer);
    ~saveCache();
    //姓名
    //证件号
    //客户编号
    //银行卡号
    //手机号
    //最小层级
    //关联标识
    //邻结点
    //子结点集合
    //父结点集合
    void fileSaveAsFile(QString name);
private:
    QMap<QString, ItemBase*> m_itemContainer;
    void voluation(TiXmlElement *e, ItemBase *_item, QString str, LINKTYPE t);//给当前的结点赋值

};

#endif // SAVECACHE_H

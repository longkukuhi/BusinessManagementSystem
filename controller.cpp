#include "model.h"
#include "data.h"
#include "controller.h"
#include <QDate>
#include <cstring>
#include <iostream>
using namespace std;

//0:收集箱 1:today 2:tomorrow 3:task list 4:date 5:everyday 6:list 7:finished
indexdata* getIndex()
{
    char fileName[]="index.dat";
    indexdata* data=getIndexData(fileName);
    return data;
}

char* getFileName(int type)
{
    char* fileName=new char[30];
    switch (type)
    {
        case 0:strcpy(fileName,"collect.dat");break;
        case 1:strcpy(fileName,"today.dat");break;
        case 2:strcpy(fileName,"tomorrow.dat");break;
        case 3:strcpy(fileName,"task.dat");break;
        case 4:strcpy(fileName,"date.dat");break;
        case 5:strcpy(fileName,"everyday.dat");break;
        case 6:strcpy(fileName,"list.dat");break;
        case 7:strcpy(fileName,"finished.dat");break;
    }
    return fileName;
}


timedata* getData(indexdata index,int beginIndex,int number)
{
    char fileName[30];
    strcpy_s(fileName,index.name);
    int total=index.total;
    if (total==0) return 0;

    if (total-beginIndex<number) number=total-beginIndex;

    timedata* array=readData(fileName,beginIndex,number);
    return array;
}


void prepare()
{
    char all[]="all.dat";
    QDate now=QDate::currentDate();
    indexdata* index=getIndex();
    if (now!=index[1].lastChange)
    {
       // cout<<"flag!"<<endl;
        clear(all);
       // clear(index[7].name);
        collectData(index[1].name,all);
        collectData(index[2].name,all);
        collectData(index[4].name,all);
        collectData(index[5].name,all);
        index = sentData(index);
        int i;
        for (i=0;i<8;i++)
            index[i].lastChange=now;
        setIndexData("index.dat",index);
    }
}

void addOneData(timedata data)
{
    char* fileName;
    data.type;
    fileName=getFileName(data.type);
    indexdata* index = getIndex();
    index[data.type].total++;
    data.index=index[data.type].lastIndex+1;
    int i;
    for (i=0;i<8;i++)
        index[i].lastIndex++;
    add(data,fileName);
    setIndexData("index.dat",index);

}

void finishOne(int type, int index)
{
    char* fileName;
    fileName=getFileName(type);
    indexdata* indexData=getIndex();
    if (type!=7)
    addToFinished(fileName,index);
  //  printFile(7);
    del(index,fileName);
    indexData[type].total--;
    if (type!=7)
    indexData[7].total++;
    setIndexData("index.dat",indexData);
}

void printFile(int type)
{
    char *fileName=getFileName(type);
    indexdata* index=getIndex();
    cout<<"index :"<<endl;
    int i;
    for (i=0;i<8;i++)
        cout<<i<<": total: "<<index[i].total<<endl;
    cout<<"file data:"<<endl;
    timedata* data=getData(index[type],0,index[type].total);
    int number;
    if (data==0)
    {
        cout<<"none!"<<endl;
        return ;
    }
    number=_msize(data)/sizeof data[0];
    for (i=0;i<number;i++)
        cout<<i<<": title: "<<data[i].title<<" index: "<<data[i].index<<endl;
}

void editDataState(timedata data, int origin)
{
    char* fileName;
    if (data.type==origin)
    {
      //  cout<<"ready to edit index: "<<data.index<<endl;
        fileName=getFileName(data.type);
        edit(data,fileName);
    }
    else
    {
        //删除原纪录，转移原纪录，更新索引
        indexdata* index=getIndex();
        fileName=getFileName(origin);
        del(data.index,fileName);
        fileName=getFileName(data.type);
        add(data,fileName);
        index[origin].total--;
        index[data.type].total++;
        setIndexData("index.dat",index);
    }
}

void resetData()
{
    if (!checkFlags())
    {
       // cout<<"first time!"<<endl;
        char* fileName;
        indexdata* index=new indexdata[8];
        int i;
        for (i=0;i<8;i++)
        {
            fileName=getFileName(i);
            clear(fileName);
            strcpy(index[i].name,fileName);
            index[i].lastChange=QDate::currentDate();
            index[i].lastIndex=0;
            index[i].total=0;
        }
        setIndexData("index.dat",index);
        setFlags(true);
    }
}

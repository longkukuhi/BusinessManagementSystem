#ifndef CONTROLLER_H
#define CONTROLLER_H 

#include "model.h"
#include "data.h"
//a
indexdata* getIndex();//return the index data,return type is an array

timedata* getData(indexdata index,int beginIndex,int number);
//返回一个timedata的数组，将从beginIndex开始的number条记录打包成数组返回
//total为文件的记录总条数

void prepare();
//程序运行时的信息准备

void addOneData(timedata data);
//向文件添加一条记录

void finishOne(int type,int index);
//完成一个日程

char* getFileName(int type);
//返回类型对应的文件名

void printFile(int type);
//print a file

void editDataState(timedata data,int origin);
//编辑一条记录


void resetData();
//run for the first time


#endif

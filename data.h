#ifndef DATA_H
#define DATA_H 

#include <QDate>
//a
struct timedata
{
	int index;
    int type;
    char title[100];
    char detail[500];
    QDate begin;
    QDate end;
};


struct indexdata
{
    char name[30];
    int total;
    QDate lastChange;
    int lastIndex;
};
#endif

#include <iostream>
#include <fstream>
#include "data.h"
#include <QDate>
#include <cstring>
using namespace std;

int add(timedata dat,char* fileName)
{
	ofstream file;
	file.open(fileName, ios_base::out|ios_base::app|ios_base::binary);
	if (!file.is_open())
	{
        cerr<<"error:can't open the file: from add";
        cerr<<fileName<<endl;
		return 1;
	}
	file.write((char *)&dat, sizeof dat);
	file.close();
	return 0;
}

timedata search(int index,char* fileName)
{
	ifstream file;
	timedata result;
	file.open(fileName, ios_base::in|ios_base::binary);
	if (!file.is_open())
	{
        cerr<<"error:can't open the file: ";
        cerr<<fileName<<endl;

	}
    while (!file.eof())
    {
        file.read((char*)&result,sizeof result);
        if (result.index==index)
            break;
    }
	file.close();
	return result;
}

int edit(timedata dat,char* fileName)
{
	fstream file;
	file.open(fileName, ios_base::in|ios_base::out|ios_base::binary);
	if (!file.is_open())
	{
        cerr<<"error:can't open the file: ";
        cerr<<fileName<<endl;
		return 1;
	}
    timedata temp;
    while (!file.eof())
    {
        file.read((char*)&temp,sizeof temp);
      //  cout<<"read index: "<<temp.index<<endl;
        if (!file.eof())
        if (temp.index==dat.index)
        {
          //  cout<<"write index: "<<temp.index<<endl;
            int place=sizeof temp;
            file.seekp(-place,ios_base::cur);
            file.write((char*)&dat,sizeof temp);
            break;
        }
    }
	file.close();
	return 0;
}

int del(int index,char* fileName)
{
	fstream temp,file;
	file.open(fileName,ios_base::in|ios_base::binary);
	if (!file.is_open())
	{
        cerr<<"error:can't open the file: ";
        cerr<<fileName<<endl;
		return 1;
	}
	temp.open("temp.dat",ios_base::out|ios_base::binary);
	if (!temp.is_open())
	{
        cerr<<"error:can't open the file: ";
        cerr<<fileName<<endl;
		return 1;
	}
	timedata t;
	while (!file.eof())
	{
		file.read((char *)&t,sizeof t);
        if (t.index==index)
        {
            continue;
        }
        if (file.eof()) continue;
		temp.write((char *)&t,sizeof t);
	}
	file.close();
	temp.close();
	file.open(fileName,ios_base::out|ios_base::binary);
	temp.open("temp.dat",ios_base::in|ios_base::binary);
	file<<temp.rdbuf();
	file.close();
	temp.close();
	return 0;
}

indexdata* getIndexData(char* fileName)
{
    int i;
    ifstream file;
    file.open(fileName,ios_base::in|ios_base::binary);
    if (!file.is_open())
    {
        cerr<<"error:can't open the file: from getIndexData";
        cerr<<fileName<<endl;
        return 0;
    }
    indexdata* array = new indexdata[8];
    indexdata temp;
    for (i=0;i<8;i++)
        file.read((char *)&array[i],sizeof temp);
    return array;
}

int setIndexData(char* fileName,indexdata* index)
{
    ofstream file;
    file.open(fileName,ios_base::out|ios_base::binary);
    if (!file.is_open())
    {
        cerr<<"error:can't open the file: from setIndexData";
        cerr<<fileName<<endl;
        return 1;
    }
    int i;
    indexdata temp;
    for (i=0;i<8;i++)
    {
        temp=index[i];
        file.write((char*)&temp,sizeof temp);
    }
    file.close();
    return 0;
}


timedata* readData(char* fileName,int begin,int total)
{
    if (total==0) return 0;
    ifstream file;
    file.open(fileName,ios_base::in|ios_base::binary);
    if (!file.is_open())
    {
        cerr<<"error:can't open the file: from readData";
        cerr<<fileName<<endl;
        return 0;
    }

    timedata* array =new timedata[total];
    int i;
    timedata temp;
    streampos place=begin*sizeof temp;
    file.seekg(place);
    for (i=0;i<total;i++)
        file.read((char*)&array[i],sizeof temp);


    file.close();
    return array;

}

void clear(char* fileName)
{
    static int time=0;
    time++;
    ofstream file;
    file.open(fileName,ios_base::out|ios_base::trunc);
    if (!file.is_open())
    {
        cerr<<"error:can't open the file: from clear,time:"<<time;
        cerr<<fileName<<endl;
    }
    file.close();

}

void collectData(char* origin,char* target)
{
    ifstream oldFile;
    oldFile.open(origin,ios_base::in|ios_base::binary);
    ofstream newFile;
    newFile.open(target,ios_base::out|ios_base::app|ios_base::binary);
    if (!oldFile.is_open()||!newFile.is_open())
    {
        cerr<<"error:can't open the file: from collectdata";
    }
    newFile<<oldFile.rdbuf();
    oldFile.close();
    newFile.close();
}

indexdata* sentData(indexdata* index)
{
    //0:收集箱 1:today 2:tomorrow 3:task list 4:date 5:everyday 6:list 7:finished

    //clear origin
    clear(index[1].name);
    clear(index[2].name);
    clear(index[4].name);
    //files

    ofstream today,tomorrow,date;
    ifstream all;
    //open files
    today.open(index[1].name,ios_base::out|ios_base::binary);
    tomorrow.open(index[2].name,ios_base::out|ios_base::binary);
    date.open(index[4].name,ios_base::out|ios_base::binary);
    all.open("all.dat",ios_base::in|ios_base::binary);

    if (!today.is_open()||!tomorrow.is_open()||date.is_open()||!all.is_open())
    {

        if (!today.is_open()) cout<<"error:can't open the file: from sentdata"<<index[1].name;

        if (!tomorrow.is_open()) cout<<"error:can't open the file: from sentdata"<<index[2].name;

        if (!date.is_open()) cout<<"error:can't open the file: from sentdata"<<index[4].name;

        if (!all.is_open()) cout<<"error:can't open the file: from sentdata"<<"all.dat";

    }

    //reset index
    indexdata &tod=index[1];
    indexdata &tom=index[2];
    indexdata &dat=index[4];
    tod.total=0;
    tom.total=0;
    dat.total=0;
    //sent
    timedata temp;
    QDate now=QDate::currentDate();
    while (!all.eof())
    {
        all.read((char *)&temp,sizeof temp);
        //set types
        if (temp.type==5)
        {
            temp.type=1;
            temp.begin=now;
            temp.end=now;
            for (int i=0;i<8;i++)
            index[i].lastIndex++;
            temp.index=index[1].lastIndex;
        }
        else
        if (temp.begin<=now)
            temp.type=1;
        else if (temp.begin==now.addDays(1))
            temp.type=2;
        else temp.type=4;

        //sent to files
        if (!all.eof())
        switch (temp.type)
        {
        case 1:today.write((char*)&temp,sizeof temp);tod.total++;break;
        case 2:tomorrow.write((char*)&temp,sizeof temp);tom.total++;break;
        case 4:date.write((char*)&temp,sizeof temp);dat.total++;break;
        }
       // cout<<"index: "<<temp.index<<"sent to "<<temp.type<<endl;
    }
    tod.lastChange=now;
    tom.lastChange=now;
    dat.lastChange=now;
    all.close();
    today.close();
    tomorrow.close();
    date.close();
    return index;
}


void test_setData()
{

    indexdata data;
    ofstream file;
    int totalIndex=0;
    file.open("index.dat",ios_base::out|ios_base::binary);
    if (!file.is_open())
    {
        cerr<<"open index.dat fail";

    }
    data.total=10;
    data.lastIndex=39;
    data.lastChange=QDate::currentDate().addDays(-2);
    strcpy(data.name,"collect.dat");
    file.write((char*)&data,sizeof data);
    strcpy(data.name,"today.dat");
    file.write((char*)&data,sizeof data);
    strcpy(data.name,"tomorrow.dat");
    file.write((char*)&data,sizeof data);
    strcpy(data.name,"task.dat");
    file.write((char*)&data,sizeof data);
    strcpy(data.name,"date.dat");
    file.write((char*)&data,sizeof data);
    data.total=0;
    strcpy(data.name,"everyday.dat");
    file.write((char*)&data,sizeof data);
    strcpy(data.name,"list.dat");
    file.write((char*)&data,sizeof data);
    strcpy(data.name,"finished.dat");
    file.write((char*)&data,sizeof data);
    file.close();

    int i;
    timedata temp;
    temp.begin=QDate::currentDate();
    temp.end=QDate::currentDate().addDays(2);
    strcpy(temp.title,"test:0");
    strcpy(temp.detail,"test detail");

    file.open("collect.dat",ios_base::out|ios_base::binary);
    if (!file.is_open())
    {
        cerr<<"open collect.dat fail";
    }

    for (i=0;i<10;i++)
    {
       // temp.index=i;
        temp.type=0;
        temp.index=totalIndex;
        totalIndex++;
        file.write((char*)&temp,sizeof temp);
    }
    file.close();

    strcpy(temp.title,"test:1");
    file.open("today.dat",ios_base::out|ios_base::binary);
    if (!file.is_open())
    {
        cerr<<"open today.dat fail";
    }

    for (i=0;i<10;i++)
    {
        //temp.index=i;
        temp.type=1;
        temp.index=totalIndex;
        totalIndex++;
        file.write((char*)&temp,sizeof temp);
    }
    file.close();


    temp.begin=temp.begin.addDays(1);
    strcpy(temp.title,"test:2");
    file.open("tomorrow.dat",ios_base::out|ios_base::binary);
    if (!file.is_open())
    {
        cerr<<"open tomorrow.dat fail";
    }

    for (i=0;i<10;i++)
    {
       // temp.index=i;
        temp.type=2;
        temp.index=totalIndex;
        totalIndex++;
        file.write((char*)&temp,sizeof temp);
    }
    file.close();

    temp.begin=temp.begin.addDays(3);
    temp.end=temp.end.addDays(3);
    strcpy(temp.title,"test:4");
    file.open("date.dat",ios_base::out|ios_base::binary);
    if (!file.is_open())
    {
        cerr<<"open date.dat fail";
    }

    for (i=0;i<10;i++)
    {
        //temp.index=i;
        temp.type=4;
        temp.index=totalIndex;
        totalIndex++;
        file.write((char*)&temp,sizeof temp);
    }
    file.close();

    strcpy(temp.title,"test:3");
    file.open("task.dat",ios_base::out|ios_base::binary);
    if (!file.is_open())
    {
        cerr<<"open task.dat fail";
    }

    for (i=0;i<10;i++)
    {
        //temp.index=i;
        temp.type=3;
        temp.index=totalIndex;
        totalIndex++;
        file.write((char*)&temp,sizeof temp);
    }
    file.close();


}


void addToFinished(char *fileName, int index)
{

    timedata finishedData=search(index,fileName);
    finishedData.type=7;
    add(finishedData,"finished.dat");
}

bool checkFlags()
{
    ifstream file;
    file.open("flags.dat",ios_base::in);
    bool flag;
    file>>flag;
    file.close();
    return flag;
}

void setFlags(bool flags)
{
    ofstream file;
    file.open("flags.dat",ios_base::out);
    file<<flags;
    file.close();
}

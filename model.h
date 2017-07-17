#ifndef MODEL_H
#define MODEL_H 

#include "data.h"
//add a record to a file
int add(timedata dat,char* fileName);

//search specific record in a file
timedata search(int index,char* fileName);

//edit a specific record in a file
int edit(timedata dat,char* fileName);
//a
//delete a specific record in a file
int del(int index,char* fileName);

//read index data
indexdata* getIndexData(char* fileName);

//write index data
int setIndexData(char* fileName,indexdata*);

//read specific number of data from file certain location
timedata* readData(char* fileName,int begin,int total);

//clear the data of a file
void clear(char* fileName);

//collect data from origin file to target file
void collectData(char* origin,char* target);

//sent data from all.dat to smaller file,return new index
indexdata* sentData(indexdata* index);

void test_setData();

//add one data to finished
void addToFinished(char* fileName,int index);

bool checkFlags();
//return the state of run flags

void setFlags(bool flag);
//set flag state

#endif




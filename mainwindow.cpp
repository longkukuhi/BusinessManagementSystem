#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "task.h"
#include "data.h"
#include "controller.h"
#include <QSpacerItem>
#include <iostream>
#include "adddialog.h"
#include <QLabel>
using namespace std;




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->addButton->setFlat(true);
    ui->nextBotton->setFlat(true);
    ui->lastButton->setFlat(true);

    for (int i=0;i<8;i++)
        page[i]=0;
    //connect
    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),ui->stackedWidget,SLOT(setCurrentIndex(int)));
    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(setTotal(int)));
    connect(ui->stateTab_1,SIGNAL(currentChanged(int)),this,SLOT(changeTab(int)));
    //view setting
    layout[0]=ui->todolayout_0;
    layout[1]=ui->todolayout_1;
    layout[2]=ui->todolayout_2;
    layout[3]=ui->todolayout_3;
    layout[4]=ui->todolayout_4;
    layout[5]=ui->todolayout_5;
    layout[6]=ui->todolayout_6;
    layout[7]=ui->todolayout_7;
    totalData=ui->page;
    ui->listWidget->setCurrentRow(1);
    //add data
    setUpData();


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setUpData()
{
    //绘制逻辑
    int i,j,number;
    indexdata* index=getIndex();
    timedata* data;
    for (i=0;i<8;i++)
    {

            redraw(i,0);
//        data=getData(index[i],0,7);

//        if (data==0) continue;

//        number=_msize(data)/sizeof data[0];
//        for (j=0;j<number;j++)
//        {
//            task* temp=getTask(data[j]);
//            connect(temp,SIGNAL(editDone()),this,SLOT(editRedraw()));
//            connect(temp,SIGNAL(done(int,int)),this,SLOT(finish(int,int)));
//            layout[i]->addWidget(temp);
//        }
//        layout[i]->addStretch();
    }


}

task* MainWindow::getTask(timedata data)
{
    task* temp=new task();
    QString text=QString(data.title);

    temp->setIndex(data.index);
    temp->setData(data);
    temp->setTitle(text);
    temp->setTime(data.begin,data.end);

    return temp;
}

void MainWindow::on_addButton_clicked()
{
    addDialog* addWindow = new addDialog;
    connect(addWindow,SIGNAL(addData(timedata)),this,SLOT(addNewData(timedata)));
    addWindow->exec();
}

void MainWindow::addNewData(timedata data)
{

    //type:0:collect 1:today 2:tomorrow 3:task list 4:date 5:list
    if (data.type==4)
        if (data.begin==QDate::currentDate())
            data.type=1;
        else if (data.begin==QDate::currentDate().addDays(1))
            data.type=2;
    addOneData(data);
    int currentPage=page[data.type];
    redraw(data.type,currentPage);
    if (data.type==5)
    {
        data.type=1;
        addOneData(data);
        redraw(1,page[1]);
    }
}

void MainWindow::redraw(int type, int currentpage)
{
    int i;
    int total;
    int begin;
    int originNumber;
    QWidget* temp;
    begin=currentpage*7;
    int number=7;
    indexdata* index=getIndex();
    if (index[type].total<begin+7)
        number=index[type].total-begin;

    if (number==0)
        if (currentpage>0)
        {
            currentpage--;
            begin=currentpage*7;
            number=7<index[type].total?7:index[type].total;
            page[type]--;
        }

    timedata* data=getData(index[type],begin,number);
    if (data==0)
    {

        originNumber=layout[type]->count();

        for (i=0;i<originNumber;i++)
        {

            temp=layout[type]->takeAt(0)->widget();
            layout[type]->removeWidget(temp);
            delete temp;
        }
        return ;
    }

    total=_msize(data)/sizeof data[0];

    originNumber=layout[type]->count();

    for (i=0;i<originNumber;i++)
    {
        temp=layout[type]->takeAt(0)->widget();
        layout[type]->removeWidget(temp);
        delete temp;
    }

    for (i=0;i<total;i++)
    {
        temp=getTask(data[i]);
        connect(temp,SIGNAL(editDone(int)),this,SLOT(editRedraw(int)));
        connect(temp,SIGNAL(done(int,int)),this,SLOT(finish(int,int)));
        layout[type]->addWidget(temp);
    }
        layout[type]->addStretch();
    setTotal(type);

}

void MainWindow::setTotal(int index)
{
    QString text;
    indexdata* indexArray = getIndex();
    int total;
    total=indexArray[index].total;
    text=tr("共");
    text+=QString::number(total);
    text+=tr("条");
    totalData->setText(text);

}


void MainWindow::on_lastButton_clicked()
{
    int type=ui->listWidget->currentRow();
    if (type==1&&ui->stateTab_1->currentIndex()==1)
        type=7;
    int currentPage = page[type];
    if (currentPage==0) return ;
    redraw(type,currentPage-1);
    page[type]--;
}

void MainWindow::on_nextBotton_clicked()
{
    int type=ui->listWidget->currentRow();
    if (type==1&&ui->stateTab_1->currentIndex()==1)
        type=7;
    int currentPage = page[type];
    indexdata* index=getIndex();
    int total=index[type].total;
    if (total>(currentPage+1)*7)
    {
        redraw(type,currentPage+1);
        page[type]++;
    }
}

void MainWindow::finish(int state, int index)
{

    int type=ui->listWidget->currentRow();
    if (type==1&&ui->stateTab_1->currentIndex()==1)
        type=7;
    int currentPage=page[type];
//    cout<<"finish : "<<index<<endl;
    finishOne(type,index);
//    printFile(type);

    redraw(type,currentPage);
}

void MainWindow::changeTab(int tab)
{
    if (tab==0) redraw(1,0);
    else redraw(7,0);
}

void MainWindow::editRedraw(int index)
{
    int type=ui->listWidget->currentRow();
    if (type==1&&ui->stateTab_0->currentIndex()==1)
        type=7;
    int currentPage=page[type];
    redraw(type,currentPage);
    redraw(index,page[index]);
}

void MainWindow::showSize()
{
    cout<<"height: "<<ui->centralWidget->height()<<endl;
    cout<<"width: "<<ui->centralWidget->width()<<endl;
    cout<<"list: height: "<<ui->listWidget->height()<<endl;
}

#include "task.h"
#include "ui_task.h"
#include <QString>
#include <QDate>
#include "controller.h"
#include <iostream>
#include <adddialog.h>
using namespace std;

task::task(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::task)
{
    ui->setupUi(this);
}

task::~task()
{
    delete ui;
}


void task::setTitle(QString text)
{
    ui->label->setText(text);
}


void task::setTitle(char * title)
{
    QString text = QString(title) ;
    ui->label->setText(text);
}

void task::setTime(QDate time,QDate end)
{
    QDate now = QDate::currentDate();
    if (data.type==4||data.type==7)
    {
        ui->checkBox->setText(time.toString("yy-MM-dd"));
        return ;
    }
    else if (data.type==2)
    {
        ui->checkBox->setText(time.toString("MM-dd"));
        ui->checkBox->setStyleSheet("color:green");
        return ;
    }
    else if (data.type==5||data.type==6||data.type==3)
    {
        ui->checkBox->setText(time.toString("MM-dd"));
        return ;
    }
    else
    if (time<now&&now>end)
    {
        ui->checkBox->setText(time.toString("MM-dd"));

        ui->checkBox->setStyleSheet("color:rgb(221,149,86)");
    }
    else if (now==end)
    {
        ui->checkBox->setText(time.toString("MM-dd"));
    }
    else if (now<end)
    {
        QString text=QString("至");
        text=text+end.toString("MM-dd");
        ui->checkBox->setText(text);
        ui->checkBox->setStyleSheet("color:green");
    }
}

void task::on_checkBox_stateChanged(int arg1)
{
    int state = ui->checkBox->checkState();
    if (state==2)
        emit done(state,index);
}

void task::setIndex(int i)
{
    index=i;
}

void task::setData(timedata Data)
{
    data=Data;
}

void task::on_more_clicked()
{
    addDialog* editDlg=new addDialog(1,data);
    connect(editDlg,SIGNAL(editData(timedata)),this,SLOT(sentEditData(timedata)));
    editDlg->exec();
}

void task::sentEditData(timedata editedData)
{
    editedData.index=data.index;
    int origin=data.type;
    data=editedData;
    editDataState(editedData,origin);
    printFile(editedData.type);
    emit editDone(editedData.type);
}

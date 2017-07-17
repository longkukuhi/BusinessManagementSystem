#include "adddialog.h"
#include "ui_adddialog.h"
#include <QDate>
#include <QMessageBox>
#include <string>
#include <cstring>
#include <iostream>
using namespace std;

addDialog::addDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addDialog)
{
    ui->setupUi(this);
    QDate now = QDate::currentDate();
    this->setWindowTitle(tr("添加日程"));
    ui->beginDate->setDate(now);
    ui->endDate->setDate(now);
    type=0;
}

addDialog::addDialog(int t,timedata data,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addDialog)
{
    type=t;
    ui->setupUi(this);
    QDate now = QDate::currentDate();
    ui->beginDate->setDate(now);
    ui->endDate->setDate(now);
    if (type==1)
    {
        this->setWindowTitle(tr("查看详情"));
        ui->confirm->setText(tr("保存变动"));
        ui->title->setText(data.title);
        ui->detail->setText(data.detail);
        ui->type->setCurrentIndex(data.type+1);
        ui->beginDate->setDate(data.begin);
        ui->endDate->setDate(data.end);
    }
}

addDialog::~addDialog()
{
    delete ui;
}

void addDialog::on_confirm_clicked()
{
    if (ui->title->text()=="")
    {
        QMessageBox::warning(this,tr("error"),tr("标题未填写！"),QMessageBox::Yes);
        return ;
    }
    else if (ui->type->currentIndex()==0)
       {
         QMessageBox::warning(this,tr("error"),tr("类型未填写！"),QMessageBox::Yes);
         return ;
       }
    else if (ui->beginDate->date()>ui->endDate->date())
    {
        QMessageBox::warning(this,tr("error"),tr("日期错误！"),QMessageBox::Yes);
        return ;
    }
    else if (ui->beginDate->date()<QDate::currentDate())
    {
        if (type==1&&ui->type->currentIndex()==2) return;
        QMessageBox::warning(this,tr("error"),tr("开始日期错误！"),QMessageBox::Yes);
        return ;
    }
    //start to add data
    std::string title=ui->title->text().toStdString();
    std::string detail=ui->detail->toPlainText().toStdString();
    timedata data;
    strcpy(data.title,title.c_str());
    strcpy(data.detail,detail.c_str());
    data.begin=ui->beginDate->date();
    data.end=ui->endDate->date();
    data.type=ui->type->currentIndex()-1;
    if (type==1)
        emit editData(data);
    else emit addData(data);
    accept();

}

void addDialog::on_cancel_clicked()
{
    close();
}

void addDialog::on_type_currentIndexChanged(int index)
{
    QDate now =QDate::currentDate();
    switch (index)
    {
        case 1://collect
            ui->beginDate->setEnabled(false);
            ui->endDate->setEnabled(false);
        break;
        case 2://today
            ui->beginDate->setDate(now);
            ui->beginDate->setEnabled(false);
        break;
        case 3://tomorrow
            now=now.addDays(1);
            ui->beginDate->setDate(now);
            ui->endDate->setDate(now);
            ui->beginDate->setEnabled(false);
        break;
        case 6://date
            ui->beginDate->setEnabled(false);
            ui->endDate->setEnabled(false);
        break;
    }
}

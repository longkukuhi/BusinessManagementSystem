#ifndef TASK_H
#define TASK_H

#include <QWidget>
#include <QString>
#include "data.h"
//a
namespace Ui {
class task;
}

class task : public QWidget
{
    Q_OBJECT

public:
    explicit task(QWidget *parent = 0);
    void setTitle(char*);
    void setTime(QDate time,QDate end);
    void setIndex(int i);
    void setTitle(QString);
    void setData(timedata Data);
    ~task();

private:
    Ui::task *ui;
    int index;
    timedata data;

signals:
    void done(int state,int index);
    void editDone(int);

private slots:
    void on_checkBox_stateChanged(int arg1);
    void on_more_clicked();
    void sentEditData(timedata editedData);
};

#endif // TASK_H

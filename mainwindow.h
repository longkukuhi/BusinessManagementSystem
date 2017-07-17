#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBoxLayout>
#include <QDate>
#include "task.h"
#include "data.h"
#include <QLabel>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showSize();
//a

private slots:
    void on_addButton_clicked();

    void addNewData(timedata);

    void setTotal(int);

    void on_lastButton_clicked();

    void on_nextBotton_clicked();

    void finish(int state,int index);

    void changeTab(int tab);

    void editRedraw(int index);

private:
    int page[8];

    QVBoxLayout* layout[8];

    QLabel* totalData;

    Ui::MainWindow *ui;

    void setUpData();

    task* getTask(timedata data);

    void redraw(int type,int page);


};

#endif // MAINWINDOW_H

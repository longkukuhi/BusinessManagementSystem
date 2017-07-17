#include "mainwindow.h"
#include <QApplication>
#include "controller.h"
#include <iostream>
#include <QDebug>

using namespace std;

int main(int argc, char *argv[])
{
    resetData();
    prepare();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

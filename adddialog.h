#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <data.h>
//a
namespace Ui {
class addDialog;
}

class addDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addDialog(QWidget *parent = 0);
     addDialog(int t, timedata data,QWidget *parent = 0);
    ~addDialog();

private:
    Ui::addDialog *ui;
    int type;

signals:
    void addData(timedata);
    void editData(timedata);
private slots:
    void on_confirm_clicked();
    void on_cancel_clicked();
    void on_type_currentIndexChanged(int index);
};

#endif // ADDDIALOG_H

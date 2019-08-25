#ifndef CLOCKDIALOG_H
#define CLOCKDIALOG_H

#include <QDialog>
#include "todaydialog.h"

namespace Ui {
class ClockDialog;
}

class ClockDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClockDialog(QWidget *parent = 0);
    ~ClockDialog();

signals:
    void addclockSignal(QString str);

private slots:
    void reject();

    void on_todayButton_clicked();

    void on_exitButton_clicked();

    void on_addButton_clicked();

    void CkeckButtonChangedSlot();

    void on_okButton_clicked();

private:
    Ui::ClockDialog *ui;
    todayDialog *today = new todayDialog;

    int clocknum = 0;
};

#endif // CLOCKDIALOG_H

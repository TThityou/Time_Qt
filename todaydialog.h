#ifndef TODAYDIALOG_H
#define TODAYDIALOG_H

#include <QDialog>

namespace Ui {
class todayDialog;
}

class todayDialog : public QDialog
{
    Q_OBJECT

public:
    explicit todayDialog(QWidget *parent = 0);
    ~todayDialog();

private:
    Ui::todayDialog *ui;
};

#endif // TODAYDIALOG_H

#include "todaydialog.h"
#include "ui_todaydialog.h"
#include <Windows.h>
todayDialog::todayDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::todayDialog)
{
    ui->setupUi(this);
    //window和Qt的API使窗口置顶
    SetWindowPos((HWND)this->winId(), HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
    this->setWindowFlags(this->windowFlags()|Qt::WindowStaysOnTopHint|Qt::X11BypassWindowManagerHint);
}

todayDialog::~todayDialog()
{
    delete ui;
}

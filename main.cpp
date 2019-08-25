#include "time.h"
#include <QApplication>
#include <QtDebug>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //获取电脑主界面的对象
    QDesktopWidget* pDesktopWidget = QApplication::desktop();

    Time w;

    //移动居中
    w.move((pDesktopWidget->width()-w.width())/2,w.moveUpWidth);


    //背景透明
    w.setAttribute(Qt::WA_TranslucentBackground, true);


    //window和Qt的API使窗口置顶
    SetWindowPos((HWND)w.winId(), HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
    w.setWindowFlags(w.windowFlags()|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::X11BypassWindowManagerHint);

    w.show();

    return a.exec();
}

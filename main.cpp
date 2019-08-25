#include "time.h"
#include <QApplication>
#include <QtDebug>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //��ȡ����������Ķ���
    QDesktopWidget* pDesktopWidget = QApplication::desktop();

    Time w;

    //�ƶ�����
    w.move((pDesktopWidget->width()-w.width())/2,w.moveUpWidth);


    //����͸��
    w.setAttribute(Qt::WA_TranslucentBackground, true);


    //window��Qt��APIʹ�����ö�
    SetWindowPos((HWND)w.winId(), HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
    w.setWindowFlags(w.windowFlags()|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::X11BypassWindowManagerHint);

    w.show();

    return a.exec();
}

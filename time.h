#ifndef TIME_H
#define TIME_H
#include "clockdialog.h"
#include <QWidget>
#include <QPoint>
#include <Windows.h>
#include <QFontMetrics>
#include <QFont>
#include <QTimer>
#include <QPalette>
namespace Ui {
class Time;
}

class Time : public QWidget
{
    Q_OBJECT

public:
    explicit Time(QWidget *parent = 0);
    ~Time();
    //窗口移动高度
    int moveUpWidth = -8;
    int flagNum = 0;

    QTimer *rgbTimer = new QTimer;
    QTimer *bgTimer = new QTimer;
    ClockDialog *clock = new ClockDialog;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);


private slots:
    void timeSlot();
    void rgbSlot();
    void timeclockSlot(QString str);
    void timeoutSlot();

private:
    Ui::Time *ui;
    QPoint p;
    QPalette pa;

    bool flagShowFullScreen = false;

};

#endif // TIME_H

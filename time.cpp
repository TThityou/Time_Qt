#include "time.h"
#include "ui_time.h"
#include <QTime>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <QMouseEvent>
#include <QToolTip>
#include <QDesktopWidget>
#include <QPalette>
#include <QPixmap>
#include <QtGlobal>//随机数
#include <QColor>
#include <QMessageBox>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFileInfo>
#include <QDir>

Time::Time(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Time)/*定时器定义*/
{
    ui->setupUi(this);
    //定义定时器每0.1秒更新时间
    QTimer *timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&Time::timeSlot);
    timer->start(100);

    connect(clock,&ClockDialog::addclockSignal,this,&Time::timeclockSlot);




//test----------------------------------------------------------
//    clock->setWindowModality(Qt::WindowModal);
//    clock->exec();
}

Time::~Time()
{
    delete ui;
}

void Time::timeSlot()/*更新时间*/
{
    QDateTime local(QDateTime::currentDateTime());
    QString time = local.toString("yyyy-MM-dd HH:mm:ss");
    ui->timeLabel->setText(time);
    //qDebug() << "Local time is:" << local;
}

void Time::mousePressEvent(QMouseEvent *event)/*移动、归位、置顶、取消置顶*/
{
    if(flagShowFullScreen == true)
    {
        return;
    }

    //光标在窗口的位置
    p = event->globalPos() - this->frameGeometry().topLeft();/*移动*/

    if(event->button() == Qt::MidButton)/*归位*/
    {
        QDesktopWidget* pDesktopWidget = QApplication::desktop();
        this->move((pDesktopWidget->width() - this->width())/2, this->moveUpWidth);
        qDebug()<<"TopMid";
    }

    else if (event->button() == Qt::RightButton)/*置顶、取消置顶*/
    {
        static int click = 2;

        if(click%2)
        {
            //置顶
            click =2;

            SetWindowPos((HWND)this->winId(), HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
            setWindowFlags(this->windowFlags()|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::X11BypassWindowManagerHint);

            qDebug()<<"StayOn";
        }else
        {
            click =3;

            SetWindowPos((HWND)this->winId(),HWND_BOTTOM,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
            setWindowFlags(this->windowFlags()|Qt::FramelessWindowHint);//CustomizeWindowHint

            qDebug()<<"Hint";
        }

        {
            //            qDebug()<<"mousePressEvent - RightButton"<<endl
            //                    <<event->globalPos();//光标在window窗口的位置
            //            qDebug()<<this->frameGeometry().topLeft();//窗口在window界面的位置
            //            qDebug()<<p;//光标在窗口的位置
        }
    }
}

void Time::mouseMoveEvent(QMouseEvent *event)/*移动*/
{
    //全屏则return
    if(flagShowFullScreen == true)
    {
        return;
    }

    move(event->globalPos()-p);
    //qDebug()<<"mouseMoveEvent - left"<<event->globalPos()-p;
    qDebug()<<"more";
}

void Time::mouseDoubleClickEvent(QMouseEvent *event)/*关闭、左上角*/
{
    if(event->button() == Qt::RightButton)/*关闭*/
    {
        close();
        qDebug()<<"close";
    }
    else if(event->button() == Qt::LeftButton)/*左上角*/
    {
        //全屏则return
        if(flagShowFullScreen == true)
        {
            return;
        }

        this->move(0,this->moveUpWidth);
        qDebug()<<"topleft";
    }
}

void Time::keyPressEvent(QKeyEvent *event)/*键盘、全屏、图片切换*/
{
    if(event->key() == Qt::Key_Return)
    {
        static int flags = 3;
        if(flags%2)
        {
            flags = 2;
            //通过时间产生随机数，实现跟换前景图的效果
            QTime t;
            t = QTime::currentTime();
            qsrand(t.msec()+t.second());
            //改变样式表，改变前景图，改变字体尺寸
            ui->timeLabel->setStyleSheet("border-image  : url(:/bg/background/"+QString::number(qrand()%10)+".jpg);"
                                                                                                            "font          :  160pt \"Vivaldi\" ");
            //通过定时器在全屏状态下切换图片
            bgTimer->start((t.second()%10+1)*10000);//10s以上
            connect(bgTimer,&QTimer::timeout,ui->timeLabel,
                    [=]()
            {
                qsrand(t.currentTime().msec()+t.currentTime().second()*1000);
                ui->timeLabel->setStyleSheet("border-image: url(:/bg/background/"+QString::number(qrand()%10)+".jpg);"
                                                                                                              "font:  160pt \"Vivaldi\" ");
            });
            {
                //ui->timeLabel->setStyleSheet("border-image: url(:/bg/background/"+QString::number(qrand()%10)+".jpg);"
                //                             "font:  160pt \"Vivaldi\";"
                //                             "color:rgb("+QString::number(qrand()%256)+","+QString::number(qrand()%256)+","+QString::number(qrand()%256)+")");
                //ui->timeLabel->setStyleSheet("border-image: url(:/img/back.jpg)");
                //ui->timeLabel->setStyleSheet("border-image: url(:/img/background.jpg)");//文件太大，程序崩溃
                //ui->timeLabel->setStyleSheet("background-font:rgb(0, 255, 127)");

                //通过创建新字体格式，更改字体
                //const QFont showFullFont("Vivaldi",160);
                //ui->timeLabel->setFont(showFullFont);

                //更改字体颜色
                //QPalette pa;
                //pa.setColor(QPalette::WindowText,Qt::red);
                //ui->timeLabel->setPalette(pa);
            }

            this->showFullScreen();

            //------------------------------------
            flagShowFullScreen = true;//标志全屏flag
            //------------------------------------

            ui->timeLabel->setCursor(Qt::BlankCursor);//隐藏光标
        }
        else
        {
            flags = 3;

            bgTimer->stop();

            ui->timeLabel->setStyleSheet("border-image: ");

            //通过创建新字体格式，更改字体
            //const QFont localLableFont("Vivaldi",24);//"华文隶书,24,-1,5,50,0,0,0,0,0"
            //ui->timeLabel->setFont(localLableFont);

            this->showNormal();
            ui->timeLabel->setCursor(Qt::SizeAllCursor);//设置光标图式
            //------------------------------------
            flagShowFullScreen = false;
            //------------------------------------

            QDesktopWidget* pDesktopWidget = QApplication::desktop();
            this->move((pDesktopWidget->width() - this->width())/2, this->moveUpWidth);

        }
    }
    else if (event->key() == Qt::Key_F2)
    {
        clock->setWindowModality(Qt::WindowModal);
        clock->exec();
        //==show();但是特点是阻塞祖宗窗口
    }
}

void Time::enterEvent(QEvent *event)
{
    rgbTimer->start(40);
    connect(rgbTimer,&QTimer::timeout,this,&Time::rgbSlot);
}

void Time::leaveEvent(QEvent *event)
{
    rgbTimer->stop();
    pa.setColor(QPalette::WindowText,Qt::black);
    ui->timeLabel->setPalette(pa);
}

void Time::rgbSlot()
{
    static int rgb = 0;
    //颜色要有一个循环才不会出现突然换色，显得不舒服
    //    红		黄		橙		绿		蓝		紫		红       黑
    //R	255		255		0		0		0		255		255     0
    //G	0		255		255		255		0		0		0       0
    //B	0		0		0		255		255		255		0       0

    //	256		512		768		1024	1280	1536	1792    2048
    if(rgb < 256)//r u
    {
        pa.setColor(QPalette::WindowText,QColor::fromRgb(rgb%256,0,0));
        ui->timeLabel->setPalette(pa);
        //qDebug()<<"rgb < 256"<<QColor::fromRgb(rgb,0,0);
    }
    else if(rgb < 512)//g u
    {
        pa.setColor(QPalette::WindowText,QColor::fromRgb(255,rgb%256,0));
        ui->timeLabel->setPalette(pa);
    }
    else if(rgb < 768)//r d
    {
        pa.setColor(QPalette::WindowText,QColor::fromRgb(255-rgb%256,255,0));
        ui->timeLabel->setPalette(pa);
    }
    else if(rgb < 1024)//b u
    {
        pa.setColor(QPalette::WindowText,QColor::fromRgb(0,255,rgb%256));
        ui->timeLabel->setPalette(pa);
    }
    else if(rgb < 1280)//g d
    {
        pa.setColor(QPalette::WindowText,QColor::fromRgb(0,255-rgb%256,255));
        ui->timeLabel->setPalette(pa);
    }
    else if(rgb < 1536)//r u
    {
        pa.setColor(QPalette::WindowText,QColor::fromRgb(rgb%256,0,255));
        ui->timeLabel->setPalette(pa);
    }
    else if(rgb < 1792)//b d
    {
        pa.setColor(QPalette::WindowText,QColor::fromRgb(255,0,255-rgb%256));
        ui->timeLabel->setPalette(pa);
    }
    else if(rgb < 2048)//r d
    {
        pa.setColor(QPalette::WindowText,QColor::fromRgb(255-rgb%256,0,0));
        ui->timeLabel->setPalette(pa);
    }
    else if(rgb == 2048)
    {
        rgb=0;
    }
    rgb++;
}

void Time::timeclockSlot(QString str)
{
    flagNum++;

    //响一次的定时器
    //qDebug()<<"time clock is start!  "<<str;

    //闹铃设定的时间
    QDateTime timeclock = QDateTime::fromString(str,"yyyy/MM/dd hh:mm");
    //qDebug()<<timeclock.time()<<timeclock.date();

    //现在的时间
    QDateTime nowtime = QDateTime::currentDateTime();
    qDebug()<<"timeclock"<<timeclock;
    qDebug()<<"nowtime"<<nowtime;

    //1d == 86400s
    //1M == 2592000s
    //2147483647ms == 2147483.647s == 24 d

    int yyyy = timeclock.date().year() - nowtime.date().year();
    int MM = timeclock.date().month() - nowtime.date().month();
    int dd = timeclock.date().day() - nowtime.date().day();

    int hh = timeclock.time().hour() - nowtime.time().hour();
    int mm = timeclock.time().minute() - nowtime.time().minute();
    int ss = nowtime.time().second();
    qDebug()<<yyyy<<"/"<<MM<<"/"<<dd<<" "<<hh<<":"<<mm<<":"<<ss;

    //设定时间
    int clocktime = 1000*( 60*( mm+60*(hh+24*(dd+30*(MM+12*(yyyy))))) - ss);

    if(clocktime<0 || clocktime > 2147483647 )/*时间不对退出*/
    {
        qDebug()<<"The clock time is Error!\n Max day is 24\nMax clock is 4";
        QMessageBox::information(this,"Error","The clock time is Error!\n Max day is 24\nMax clock is 4");
        return;
    }

    if(flagNum == 1)
    {

        qDebug()<<"\n first!";
        qDebug()<<clocktime<<"ms  "<<clocktime/1000<<"s  "<<clocktime/1000/60<<"m";
        qDebug()<<clocktime/1000/60/60<<"h "<<clocktime/1000/60/60/24<<"d "<<clocktime/1000/60/60/24/30<<"M ";
        QTimer::singleShot(clocktime, this, &Time::timeoutSlot);
    }
    else if(flagNum == 2)
    {
        int clocktime = 1000*(60*(mm+60*(hh+24*(dd+30*(MM+12*(yyyy))))) - ss);
        qDebug()<<"\n second!";
        qDebug()<<clocktime<<"ms  "<<clocktime/1000<<"s  "<<clocktime/1000/60<<"m";
        qDebug()<<clocktime/1000/60/60<<"h "<<clocktime/1000/60/60/24<<"d "<<clocktime/1000/60/60/24/30<<"M ";
        QTimer::singleShot(clocktime, this, &Time::timeoutSlot);
    }
    else if(flagNum == 3)
    {
        int clocktime = 1000*(60*(mm+60*(hh+24*(dd+30*(MM+12*(yyyy))))) - ss);
        qDebug()<<"\n thirt!";
        qDebug()<<clocktime<<"ms  "<<clocktime/1000<<"s  "<<clocktime/1000/60<<"m";
        qDebug()<<clocktime/1000/60/60<<"h "<<clocktime/1000/60/60/24<<"d "<<clocktime/1000/60/60/24/30<<"M ";
        QTimer::singleShot(clocktime, this, &Time::timeoutSlot);
    }
    else if(flagNum == 4)
    {
        int clocktime = 1000*(60*(mm+60*(hh+24*(dd+30*(MM+12*(yyyy))))) - ss);
        qDebug()<<"\n four!";
        qDebug()<<clocktime<<"ms  "<<clocktime/1000<<"s  "<<clocktime/1000/60<<"m";
        qDebug()<<clocktime/1000/60/60<<"h "<<clocktime/1000/60/60/24<<"d "<<clocktime/1000/60/60/24/30<<"M ";
        QTimer::singleShot(clocktime, this, &Time::timeoutSlot);
    }
    else
    {
        qDebug()<<"The clock time is Exceed!Max clock is 4";
        QMessageBox::warning(this,"Error","Max clock is 4");
        return;
    }




}

void Time::timeoutSlot()
{

    QMediaPlayer  *music = new QMediaPlayer(this);//初始化音乐
    //QString path=QFileDialog::getOpenFileName(this,tr("选择一首歌曲"),"/","*.mp3");
    qDebug()<<QDir::currentPath()+QString::fromLocal8Bit("/飘向北方.mp3");
    QString path = QDir::currentPath()+QString::fromLocal8Bit("/飘向北方.mp3");
    QFileInfo musicFile(path);
    if(!musicFile.isFile())
    {
        QMessageBox::information(this,"MusicFile Error",QString::fromLocal8Bit("飘向北方.mp3文件不存在"));
        return;
    }
    QMediaPlaylist *musicplaylist = new QMediaPlaylist;
    musicplaylist->setCurrentIndex(1);
    musicplaylist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);

    musicplaylist->addMedia(QUrl::fromLocalFile(path));

    music->setPlaylist(musicplaylist);

    //设置媒体文件源
    //music->setMedia(QUrl::fromLocalFile(path));
    music->play();//播放歌曲

    QDateTime nowtime = QDateTime::currentDateTime();
    QMessageBox::information(this,"Time out","Now is time out !\n Music running!\nNow time is "+nowtime.toString("hh:mm"));

    music->stop();//停止播放歌曲

    delete music,musicplaylist;

    qDebug()<<"**************************************************************  time now!";
    flagNum--;
    qDebug()<<"flagNum"<<flagNum;


}




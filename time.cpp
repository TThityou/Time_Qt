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
#include <QtGlobal>//�����
#include <QColor>
#include <QMessageBox>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFileInfo>
#include <QDir>

Time::Time(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Time)/*��ʱ������*/
{
    ui->setupUi(this);
    //���嶨ʱ��ÿ0.1�����ʱ��
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

void Time::timeSlot()/*����ʱ��*/
{
    QDateTime local(QDateTime::currentDateTime());
    QString time = local.toString("yyyy-MM-dd HH:mm:ss");
    ui->timeLabel->setText(time);
    //qDebug() << "Local time is:" << local;
}

void Time::mousePressEvent(QMouseEvent *event)/*�ƶ�����λ���ö���ȡ���ö�*/
{
    if(flagShowFullScreen == true)
    {
        return;
    }

    //����ڴ��ڵ�λ��
    p = event->globalPos() - this->frameGeometry().topLeft();/*�ƶ�*/

    if(event->button() == Qt::MidButton)/*��λ*/
    {
        QDesktopWidget* pDesktopWidget = QApplication::desktop();
        this->move((pDesktopWidget->width() - this->width())/2, this->moveUpWidth);
        qDebug()<<"TopMid";
    }

    else if (event->button() == Qt::RightButton)/*�ö���ȡ���ö�*/
    {
        static int click = 2;

        if(click%2)
        {
            //�ö�
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
            //                    <<event->globalPos();//�����window���ڵ�λ��
            //            qDebug()<<this->frameGeometry().topLeft();//������window�����λ��
            //            qDebug()<<p;//����ڴ��ڵ�λ��
        }
    }
}

void Time::mouseMoveEvent(QMouseEvent *event)/*�ƶ�*/
{
    //ȫ����return
    if(flagShowFullScreen == true)
    {
        return;
    }

    move(event->globalPos()-p);
    //qDebug()<<"mouseMoveEvent - left"<<event->globalPos()-p;
    qDebug()<<"more";
}

void Time::mouseDoubleClickEvent(QMouseEvent *event)/*�رա����Ͻ�*/
{
    if(event->button() == Qt::RightButton)/*�ر�*/
    {
        close();
        qDebug()<<"close";
    }
    else if(event->button() == Qt::LeftButton)/*���Ͻ�*/
    {
        //ȫ����return
        if(flagShowFullScreen == true)
        {
            return;
        }

        this->move(0,this->moveUpWidth);
        qDebug()<<"topleft";
    }
}

void Time::keyPressEvent(QKeyEvent *event)/*���̡�ȫ����ͼƬ�л�*/
{
    if(event->key() == Qt::Key_Return)
    {
        static int flags = 3;
        if(flags%2)
        {
            flags = 2;
            //ͨ��ʱ������������ʵ�ָ���ǰ��ͼ��Ч��
            QTime t;
            t = QTime::currentTime();
            qsrand(t.msec()+t.second());
            //�ı���ʽ���ı�ǰ��ͼ���ı�����ߴ�
            ui->timeLabel->setStyleSheet("border-image  : url(:/bg/background/"+QString::number(qrand()%10)+".jpg);"
                                                                                                            "font          :  160pt \"Vivaldi\" ");
            //ͨ����ʱ����ȫ��״̬���л�ͼƬ
            bgTimer->start((t.second()%10+1)*10000);//10s����
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
                //ui->timeLabel->setStyleSheet("border-image: url(:/img/background.jpg)");//�ļ�̫�󣬳������
                //ui->timeLabel->setStyleSheet("background-font:rgb(0, 255, 127)");

                //ͨ�������������ʽ����������
                //const QFont showFullFont("Vivaldi",160);
                //ui->timeLabel->setFont(showFullFont);

                //����������ɫ
                //QPalette pa;
                //pa.setColor(QPalette::WindowText,Qt::red);
                //ui->timeLabel->setPalette(pa);
            }

            this->showFullScreen();

            //------------------------------------
            flagShowFullScreen = true;//��־ȫ��flag
            //------------------------------------

            ui->timeLabel->setCursor(Qt::BlankCursor);//���ع��
        }
        else
        {
            flags = 3;

            bgTimer->stop();

            ui->timeLabel->setStyleSheet("border-image: ");

            //ͨ�������������ʽ����������
            //const QFont localLableFont("Vivaldi",24);//"��������,24,-1,5,50,0,0,0,0,0"
            //ui->timeLabel->setFont(localLableFont);

            this->showNormal();
            ui->timeLabel->setCursor(Qt::SizeAllCursor);//���ù��ͼʽ
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
        //==show();�����ص����������ڴ���
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
    //��ɫҪ��һ��ѭ���Ų������ͻȻ��ɫ���Եò����
    //    ��		��		��		��		��		��		��       ��
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

    //��һ�εĶ�ʱ��
    //qDebug()<<"time clock is start!  "<<str;

    //�����趨��ʱ��
    QDateTime timeclock = QDateTime::fromString(str,"yyyy/MM/dd hh:mm");
    //qDebug()<<timeclock.time()<<timeclock.date();

    //���ڵ�ʱ��
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

    //�趨ʱ��
    int clocktime = 1000*( 60*( mm+60*(hh+24*(dd+30*(MM+12*(yyyy))))) - ss);

    if(clocktime<0 || clocktime > 2147483647 )/*ʱ�䲻���˳�*/
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

    QMediaPlayer  *music = new QMediaPlayer(this);//��ʼ������
    //QString path=QFileDialog::getOpenFileName(this,tr("ѡ��һ�׸���"),"/","*.mp3");
    qDebug()<<QDir::currentPath()+QString::fromLocal8Bit("/Ʈ�򱱷�.mp3");
    QString path = QDir::currentPath()+QString::fromLocal8Bit("/Ʈ�򱱷�.mp3");
    QFileInfo musicFile(path);
    if(!musicFile.isFile())
    {
        QMessageBox::information(this,"MusicFile Error",QString::fromLocal8Bit("Ʈ�򱱷�.mp3�ļ�������"));
        return;
    }
    QMediaPlaylist *musicplaylist = new QMediaPlaylist;
    musicplaylist->setCurrentIndex(1);
    musicplaylist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);

    musicplaylist->addMedia(QUrl::fromLocalFile(path));

    music->setPlaylist(musicplaylist);

    //����ý���ļ�Դ
    //music->setMedia(QUrl::fromLocalFile(path));
    music->play();//���Ÿ���

    QDateTime nowtime = QDateTime::currentDateTime();
    QMessageBox::information(this,"Time out","Now is time out !\n Music running!\nNow time is "+nowtime.toString("hh:mm"));

    music->stop();//ֹͣ���Ÿ���

    delete music,musicplaylist;

    qDebug()<<"**************************************************************  time now!";
    flagNum--;
    qDebug()<<"flagNum"<<flagNum;


}




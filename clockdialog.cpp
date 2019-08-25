#include "clockdialog.h"
#include "ui_clockdialog.h"
#include <Windows.h>
#include <QDateTime>
#include <QTime>
#include <QDate>
#include <QDesktopWidget>
#include <QtDebug>
#include <QMessageBox>

ClockDialog::ClockDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClockDialog)
{
    ui->setupUi(this);
    ui->addButton->setDisabled(true);
    ui->okButton->setDisabled(true);
    //�������Ӵ����ö�
    SetWindowPos((HWND)this->winId(), HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
    this->setWindowFlags(this->windowFlags()|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::X11BypassWindowManagerHint);

    QDateTime inLocal(QDateTime::currentDateTime());
    ui->clockTimeedit->setDateTime(inLocal);

    connect(ui->timeCkeck_1,&QCheckBox::stateChanged,this,&ClockDialog::CkeckButtonChangedSlot);
    connect(ui->timeCkeck_2,&QCheckBox::stateChanged,this,&ClockDialog::CkeckButtonChangedSlot);
    connect(ui->timeCkeck_3,&QCheckBox::stateChanged,this,&ClockDialog::CkeckButtonChangedSlot);
    connect(ui->timeCkeck_4,&QCheckBox::stateChanged,this,&ClockDialog::CkeckButtonChangedSlot);


}

ClockDialog::~ClockDialog()
{
    delete ui;
}

void ClockDialog::CkeckButtonChangedSlot( )//2ѡ�У�0ȡ��
{
    //    qDebug()<<sta;
    bool timeCkeck1 = ui->timeCkeck_1->isChecked();
    bool timeCkeck2 = ui->timeCkeck_2->isChecked();
    bool timeCkeck3 = ui->timeCkeck_3->isChecked();
    bool timeCkeck4 = ui->timeCkeck_4->isChecked();
    if(timeCkeck1 || timeCkeck2 || timeCkeck3 || timeCkeck4)
    {
        ui->addButton->setDisabled(false);
        ui->okButton->setDisabled(false);
    }
    else
    {
        ui->addButton->setDisabled(true);
        ui->okButton->setDisabled(true);
    }

}
void ClockDialog::on_todayButton_clicked()
{
    //��ʾ��������
    QDesktopWidget* pDesktopWidget = QApplication::desktop();
    today->move((pDesktopWidget->width() - today->width()), 0);
    today->show();
}

//�ر��������Ӵ��� ͬʱ �ر���������
void ClockDialog::on_exitButton_clicked()
{
    today->close();
    this->close();
}

//QDialog��Esc���Զ��������¼�
void ClockDialog::reject()
{
    today->close();
    //������close����ʧЧ!!!
    this->accept();
}

void ClockDialog::on_addButton_clicked()/*�������ʱ�䵽�б�*/
{
    ui->okButton->setDisabled(false);
    bool timeCkeck1 = ui->timeCkeck_1->isChecked();
    bool timeCkeck2 = ui->timeCkeck_2->isChecked();
    bool timeCkeck3 = ui->timeCkeck_3->isChecked();
    bool timeCkeck4 = ui->timeCkeck_4->isChecked();

    qDebug()<<ui->clockTimeedit->dateTime().toString("yyyy/MM/dd hh:mm");

    QString clocktime = ui->clockTimeedit->dateTime().toString("yyyy/MM/dd hh:mm");

    if(timeCkeck1 == true)
    {
        ui->timeCkeck_1->setText(clocktime);
    }
    if(timeCkeck2 == true)
    {
        ui->timeCkeck_2->setText(clocktime);
    }
    if(timeCkeck3 == true)
    {
        ui->timeCkeck_3->setText(clocktime);
    }
    if(timeCkeck4 == true)
    {
        ui->timeCkeck_4->setText(clocktime);
    }


}

void ClockDialog::on_okButton_clicked()/*ȷ��ʱ��*/
{
    bool timeCkeck1 = ui->timeCkeck_1->isChecked();
    bool timeCkeck2 = ui->timeCkeck_2->isChecked();
    bool timeCkeck3 = ui->timeCkeck_3->isChecked();
    bool timeCkeck4 = ui->timeCkeck_4->isChecked();


    if(timeCkeck1 == true)
    {
        emit addclockSignal(ui->timeCkeck_1->text());
        clocknum++;
    }
    if(timeCkeck2 == true)
    {
        emit addclockSignal(ui->timeCkeck_2->text());
        clocknum++;
    }
    if(timeCkeck3 == true)
    {
        emit addclockSignal(ui->timeCkeck_3->text());
        clocknum++;
    }
    if(timeCkeck4 == true)
    {
        emit addclockSignal(ui->timeCkeck_4->text());
        clocknum++;
    }
    QMessageBox::information(this, "add clock time success", "Have "+QString::number(clocknum)+" times!");
    ui->okButton->setDisabled(true);
}

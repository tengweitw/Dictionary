#include "libmanager.h"
#include "ui_libmanager.h"
#include<QMouseEvent>
//#include<QDebug>
LibManager::LibManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LibManager)
{
    ui->setupUi(this);
    this->setMouseTracking(false);
    Flag=1;
     setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);//设置主窗口无边框
}
 void LibManager::mousePressEvent(QMouseEvent *event)
 {
     if(event->button() == Qt::LeftButton)
      {
         this->setMouseTracking(true);
           mouse_press = true;
           //鼠标相对于窗体的位置（或者使用event->globalPos() - this->pos()）
           move_point = event->pos();;
      }
 }

 void LibManager::mouseReleaseEvent(QMouseEvent *event)
 {
     //设置鼠标为未被按下
         mouse_press = false;
 }

 void LibManager::mouseMoveEvent(QMouseEvent *event)
 {
     //若鼠标左键被按下
   //  qDebug()<<"mouse_press="<<event->globalPos();
         if(mouse_press)
         {
             //鼠标相对于屏幕的位置
             QPoint move_pos = event->globalPos();

            //移动主窗体位置
            this->move(move_pos - move_point);
         }
 }






LibManager::~LibManager()
{
    delete ui;
}

void LibManager::on_toolButton_clicked()
{
    this->close();
}

void LibManager::on_WeiKeiBtn_clicked()
{
    this->Flag=1;

}



void LibManager::on_ChineseBtn_clicked()
{
    Flag=8;

}

void LibManager::on_SimilarBtn_clicked()
{
    Flag=3;

}

void LibManager::on_ClanguageBtn_clicked()
{

    Flag=2;
}

void LibManager::on_HuaWeiBtn_clicked()
{

    Flag=4;
}

void LibManager::on_ProverbBtn_clicked()
{

    Flag=5;
}

void LibManager::on_PoemsBtn_clicked()
{

    Flag=6;
}

void LibManager::on_EngineerBtn_clicked()
{

    Flag=7;
}



void LibManager::on_quedingBtn_clicked()
{
    emit selectWhich(Flag);
    this->close();
}

void LibManager::replyWhichFlag(int flag)
{
    switch(flag)
    {
    case 1:ui->WeiKeiBtn->setChecked(true);
        break;
    case 2:ui->ClanguageBtn->setChecked(true);
        break;
    case 3:ui->SimilarBtn->setChecked(true);
        break;
    case 4:ui->HuaWeiBtn->setChecked(true);
        break;
    case 5:ui->ProverbBtn->setChecked(true);
        break;
    case 6:ui->PoemsBtn->setChecked(true);
        break;
    case 7:ui->EngineerBtn->setChecked(true);
        break;
    case 8:ui->ChineseBtn->setChecked(true);
        break;

    }
}

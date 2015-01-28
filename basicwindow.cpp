#include "basicwindow.h"
#include "ui_basicwindow.h"

BasicWindow::BasicWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BasicWindow)
{
    ui->setupUi(this);
    this->setMouseTracking(true);
    //设置在不按鼠标的情况下也触发鼠标移动事件，注意QMainWindow的情况:centralWidget()->setMouseTracking(true);
    isLeftPressed=false;//左键是否按下标志
    curPos=0;//标记鼠标左击时的位置
    this->setMinimumSize(400,300);//设置最小尺寸
    QCursor cursor;
    cursor.setShape(Qt::ArrowCursor);//设置鼠标为箭头形状
 //   ui->pushButton->setCursor(cursor);//当放在按钮上时，为箭头
   // cursor.setShape(Qt::OpenHandCursor);
    QWidget::setCursor(cursor);//当放在主窗口上时，为手形
 //   qDebug()<<"h="<<this->height();
    setWindowFlags(Qt::FramelessWindowHint);//设置主窗口无边框
 //   qDebug()<<this->minimumHeight();
}

BasicWindow::~BasicWindow()
{
    delete ui;
}

int BasicWindow::countFlag(QPoint p, int row)//计算当前鼠标在哪个区域
{

    if(p.y()<MARGIN)
        return 10+row;
    else if(p.y()>this->height()-MARGIN)
        return 30+row;
    else
        return 20+row;
}

void BasicWindow::setCursorType(int flag)
{
    Qt::CursorShape cursor;
       switch(flag)
       {
       case 11:
       case 33:
           cursor=Qt::SizeFDiagCursor;break;
       case 13:
       case 31:
           cursor=Qt::SizeBDiagCursor;break;
       case 21:
       case 23:
           cursor=Qt::SizeHorCursor;break;
       case 12:
       case 32:
           cursor=Qt::SizeVerCursor;break;
       case 22:
           cursor=Qt::ArrowCursor;break;
       default:
           // QApplication::restoreOverrideCursor();//恢复鼠标指针性状
            cursor=Qt::ArrowCursor;
            break;

       }
       setCursor(cursor);
}

int BasicWindow::countRow(QPoint p)
{

     return (p.x()<MARGIN)?1:(p.x()>(this->width()-MARGIN)?3:2);
}

void BasicWindow::mousePressEvent(QMouseEvent *event)
{
    emit movesignal();
    if(event->button()==Qt::LeftButton)
    {
        this->isLeftPressed=true;
        QCursor cursor;
        cursor.setShape(Qt::ClosedHandCursor);
        QApplication::setOverrideCursor(cursor);//改变鼠标指针形状
        QPoint temp=event->globalPos();
        pLast=temp;
        curPos=countFlag(event->pos(),countRow(event->pos()));
        event->ignore();
    }
}

void BasicWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(isLeftPressed)
            isLeftPressed=false;
        QApplication::restoreOverrideCursor();//恢复鼠标指针性状
        event->ignore();
}

void BasicWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
       {
           if(windowState()!=Qt::WindowFullScreen)
               setWindowState(Qt::WindowFullScreen);
           else setWindowState(Qt::WindowNoState);//恢复正常模式
       }
       event->ignore();
}

void BasicWindow::mouseMoveEvent(QMouseEvent *event)
{

    int poss=countFlag(event->pos(),countRow(event->pos()));
   //  qDebug()<<"pos="<<poss;
       setCursorType(poss);
       if(isLeftPressed)
       {
           QPoint ptemp=event->globalPos();
           ptemp=ptemp-pLast;
           if(curPos==22)//移动窗口
           {
               ptemp=ptemp+pos();
               move(ptemp);
           }
           else
           {
               QRect wid=geometry();

               switch(curPos)//改变窗口的大小
               {

               case 11:wid.setTopLeft(wid.topLeft()+ptemp);break;
               case 13:wid.setTopRight(wid.topRight()+ptemp);break;
               case 31:wid.setBottomLeft(wid.bottomLeft()+ptemp);break;
               case 33:wid.setBottomRight(wid.bottomRight()+ptemp);break;
               case 12:wid.setTop(wid.top()+ptemp.y());break;
               case 21:wid.setLeft(wid.left()+ptemp.x());break;
               case 23:wid.setRight(wid.right()+ptemp.x());break;
               case 32:wid.setBottom(wid.bottom()+ptemp.y());break;
               }
               setGeometry(wid);
           }


           pLast=event->globalPos();//更新位置
       }
       event->ignore();
}

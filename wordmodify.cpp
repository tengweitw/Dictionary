#include "wordmodify.h"
#include "ui_wordmodify.h"
#include<QFile>
#include<QTextCodec>
#include<QDateTime>
#include<QByteArray>
#include<QMessageBox>
WordModify::WordModify(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WordModify)
{
    ui->setupUi(this);
     setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);//设置主窗口无边框
     flag=0;
     row=0;
}

WordModify::~WordModify()
{
    delete ui;
}


void WordModify::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
     {
        this->setMouseTracking(true);
          mouse_press = true;
          //鼠标相对于窗体的位置（或者使用event->globalPos() - this->pos()）
          move_point = event->pos();;
     }
}

void WordModify::mouseReleaseEvent(QMouseEvent *event)
{
    //设置鼠标为未被按下
        mouse_press = false;
}

void WordModify::mouseMoveEvent(QMouseEvent *event)
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

void WordModify::on_wordCloseBtn_clicked()
{
    this->close();
}

void WordModify::replyAddWord(QString txt)
{
    ui->wordModifyLabel->setText("添加单词");
    this->flag=0;
    this->txtname=txt;
}

void WordModify::replyEditWord(QString txt,int Row)
{
    ui->wordModifyLabel->setText("编辑单词");
    this->flag=1;
    this->txtname=txt;
    this->row=Row;

    QFile *readFile =new QFile(this->txtname+".txt");
    readFile->open(QFile::ReadWrite|QIODevice::Text);
//    QTextStream in(readFile);
    QByteArray line;
    QTextCodec* gbk_codec = QTextCodec::codecForName("utf-8");
    QString temp;
    QString wordbookDef="";
    for(int i=0;i<Row;i++)
    {
        while(!gbk_codec->toUnicode(readFile->readLine().trimmed()).contains("20"));
        readFile->readLine();

    }
         line=readFile->readLine().trimmed();
         temp= gbk_codec->toUnicode(line);
         ui->wordModifyName->setText(temp);

         line=readFile->readLine().trimmed();
         temp= gbk_codec->toUnicode(line);
         ui->wordModifyPron->setText(temp);

         while(!(temp=gbk_codec->toUnicode(line=readFile->readLine().trimmed())).contains("20"))
             wordbookDef+=temp;

         ui->wordModifyDef->setText(wordbookDef);
          wordbookDef="";
    readFile->close();
 }


void WordModify::replyDeleteWord(QString txt,int Row)
{
    ui->wordModifyLabel->setText("删除单词");
    this->flag=2;
    this->txtname=txt;
    this->row=Row;

    QFile *readFile =new QFile(this->txtname+".txt");
    readFile->open(QFile::ReadWrite|QIODevice::Text);
//    QTextStream in(readFile);
    QByteArray line;
    QTextCodec* gbk_codec = QTextCodec::codecForName("utf-8");
    QString temp;
    QString wordbookDef="";
    for(int i=0;i<Row;i++)
    {
        while(!gbk_codec->toUnicode(readFile->readLine().trimmed()).contains("20"));
        readFile->readLine();

    }
         line=readFile->readLine().trimmed();
         temp= gbk_codec->toUnicode(line);
         ui->wordModifyName->setText(temp);

         line=readFile->readLine().trimmed();
         temp= gbk_codec->toUnicode(line);
         ui->wordModifyPron->setText(temp);

         while(!(temp=gbk_codec->toUnicode(line=readFile->readLine().trimmed())).contains("20"))
             wordbookDef+=temp;

         ui->wordModifyDef->setText(wordbookDef);
          wordbookDef="";
       readFile->close();
    }



bool WordModify::on_wordModifyBtn_clicked()
{
    if(this->flag==0)
    {
        QString wordbookName=ui->wordModifyName->text();
        QString wordbookPron=ui->wordModifyPron->text();
        QString wordbookDef=ui->wordModifyDef->toPlainText().toUtf8();

        QString temp;
      //  qDebug()<<"ddddddd=";

        QFile *readFile =new QFile(this->txtname+".txt");
        readFile->open(QFile::ReadWrite|QIODevice::Text);
        QTextStream in(readFile);

        QByteArray line;
        QTextCodec* gbk_codec = QTextCodec::codecForName("utf-8");

        while(!readFile->atEnd())
        {
             line=readFile->readLine().trimmed();
             temp= gbk_codec->toUnicode(line);
             if(temp==wordbookName)
             {
                 QMessageBox::about(this, tr("说明"), tr("单词本中已有该单词，可以选择编辑该单词"));
                 return false;
             }

        }
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date = current_date_time.toString("yyyy-MM-dd");

        in.setCodec("utf-8");
        in<<wordbookName<<endl;
        in<<wordbookPron<<endl;
        in<<wordbookDef<<endl;
        in<<current_date<<endl;
        in<<""<<endl;

        readFile->close();
    }
    else if(flag==1)
    {
        QString wordbookName=ui->wordModifyName->text();
        QString wordbookPron=ui->wordModifyPron->text();
        QString wordbookDef=ui->wordModifyDef->toPlainText().toUtf8();

        QString temp;


        QFile *readFile =new QFile(this->txtname+".txt");
        readFile->open(QFile::ReadWrite|QIODevice::Text);
    //    QTextStream in(readFile);

        QFile *outFile =new QFile("temp.txt");
        outFile->open(QFile::WriteOnly|QIODevice::Text);
        QTextStream out(outFile);
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date = current_date_time.toString("yyyy-MM-dd");
        while(!readFile->atEnd())
        {

             QByteArray line;
             line= readFile->readLine().trimmed();
             QTextCodec* gbk_codec = QTextCodec::codecForName("utf-8");
             QString gbk_string = gbk_codec->toUnicode(line);
             temp=gbk_string;

             if(temp.trimmed()==wordbookName)
             {
                 while(!gbk_codec->toUnicode(readFile->readLine().trimmed()).contains("20"));
                 readFile->readLine();
                 temp=gbk_codec->toUnicode(readFile->readLine().trimmed());
                 out<<wordbookName<<endl;
                 out<<wordbookPron<<endl;
                 out<<wordbookDef<<endl;
                 out<<current_date<<endl;
                 out<<"";
             }
             out.setCodec("utf-8");
             out<<temp<<endl;
        }
        readFile->close();
        outFile->close();
        //删除原文件，重命名新文件
        QFile::remove(this->txtname+".txt");
        QFile::rename("temp.txt",this->txtname+".txt");

    }
 else
    {
        QString wordbookName=ui->wordModifyName->text();


        QString temp;


        QFile *readFile =new QFile(this->txtname+".txt");
        readFile->open(QFile::ReadWrite|QIODevice::Text);
    //    QTextStream in(readFile);

        QFile *outFile =new QFile("temp.txt");
        outFile->open(QFile::WriteOnly|QIODevice::Text);
        QTextStream out(outFile);

        while(!readFile->atEnd())
        {

             QByteArray line;
             line= readFile->readLine().trimmed();
             QTextCodec* gbk_codec = QTextCodec::codecForName("utf-8");
             QString gbk_string = gbk_codec->toUnicode(line);
             temp=gbk_string;

             if(temp.trimmed()==wordbookName)
             {
                 while(!gbk_codec->toUnicode(readFile->readLine().trimmed()).contains("20"));
                 readFile->readLine();

         //        qDebug()<<"temp="<<(temp=gbk_codec->toUnicode(readFile->readLine().trimmed()));

             }
             else
             {
             out.setCodec("utf-8");
             out<<temp<<endl;
             }
        }
        readFile->close();
        outFile->close();
        //删除原文件，重命名新文件
        QFile::remove(this->txtname+".txt");
        QFile::rename("temp.txt",this->txtname+".txt");
    }


    this->close();
    return true;
}

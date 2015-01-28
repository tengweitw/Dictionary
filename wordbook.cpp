#include "wordbook.h"
#include "ui_wordbook.h"
#include<QFile>
#include<QTextStream>
#include<QByteArray>
#include<QTextCodec>
WordBook::WordBook(BasicWindow *parent) :
    BasicWindow(parent),
    ui(new Ui::WordBook)
{
    ui->setupUi(this);

    ui->wordScanBtn->setChecked(true);



//    ui->tableWidget->setItem(0,0,new QTableWidgetItem("1"));
//    ui->tableWidget->setItem(0,1,new QTableWidgetItem("2"));
//    ui->tableWidget->setItem(0,2,new QTableWidgetItem("3"));
   // ui->tableWidget->a
    //ui->tableWidget->setItem(1,0,new QTableWidgetItem("11"));

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止编辑
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选取整行
  //  ui->tableWidget->setColumnWidth(0,150);
    this->row=0;
    ui->wordEditBtn->setEnabled(false);
    ui->wordOmitBtn->setEnabled(false);
}

WordBook::~WordBook()
{
    delete ui;
}





void WordBook::on_tableWidget_doubleClicked(const QModelIndex &index)
{
  //  qDebug()<<"Ok";
}




void WordBook::on_wordMinBtn_clicked()
{
    setWindowState(Qt::WindowMinimized);
}

void WordBook::on_wordCloseBtn_clicked()
{
    this->close();
}

void WordBook::on_wordReviewBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->wordScanBtn->setChecked(false);
   // ui->wordScanBtn->setStyleSheet("background-color: rgb(152, 214, 255);");
  //  ui->wordReviewBtn->setStyleSheet("background-color: rgb(255,255, 255);");
}

void WordBook::on_wordScanBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->wordReviewBtn->setChecked(false);
 //   ui->wordReviewBtn->setStyleSheet("background-color: rgb(152, 214, 255);");
  //  ui->wordScanBtn->setStyleSheet("background-color: rgb(218,218, 218);");


}

void WordBook::replyShowWord(QString txtName)
{

    QString wordbookName;
    QString wordbookPron;
    QString wordbookDef="";
    QString wordbookFinish="";
    QString wordbookTime;
    QString temp;
  //  qDebug()<<"ddddddd="<<txtName;
    this->txt=txtName;
    QFile *readFile =new QFile(txtName+".txt");
    readFile->open(QFile::ReadWrite|QIODevice::Text);
    QTextStream in(readFile);
    int col=0;
    QByteArray line;
    QTextCodec* gbk_codec = QTextCodec::codecForName("utf-8");
    ui->tableWidget->setRowCount(0);//初始化
    while(!readFile->atEnd())
    {


        line=readFile->readLine().trimmed();
         wordbookName= gbk_codec->toUnicode(line);
//         qDebug()<<"name="<<wordbookName;
//         while(wordbookName=="")
//         {
//             line=readFile->readLine().trimmed();
//              wordbookName= gbk_codec->toUnicode(line);
//         }

         line=readFile->readLine().trimmed();
         wordbookPron=gbk_codec->toUnicode(line);
         while(!(temp=gbk_codec->toUnicode(line=readFile->readLine().trimmed())).contains("20"))
             wordbookDef+=temp;
         wordbookTime=temp;
         wordbookFinish=readFile->readLine();
         ui->tableWidget->insertRow(col);
         ui->tableWidget->setItem(col,0,new QTableWidgetItem(wordbookName));
         ui->tableWidget->setItem(col,1,new QTableWidgetItem(wordbookPron));
         ui->tableWidget->setItem(col,2,new QTableWidgetItem(wordbookDef));
         ui->tableWidget->setItem(col,3,new QTableWidgetItem(wordbookTime));
         ui->tableWidget->setItem(col,4,new QTableWidgetItem(wordbookFinish));
         wordbookDef="";

         col++;

    }
//    qDebug()<<"okkkkkk=aaaaaa";
    readFile->close();

}

void WordBook::on_wordAddBtn_clicked()
{
    WordModify *wm=new WordModify();
    wm->show();
    connect(this,SIGNAL(addWord(QString)),wm,SLOT(replyAddWord(QString)));
    emit addWord(txt);

}




void WordBook::on_wordEditBtn_clicked()
{
    WordModify *wm=new WordModify();
    wm->show();
     connect(this,SIGNAL(editWord(QString,int)),wm,SLOT(replyEditWord(QString,int)));
    emit editWord(txt,row);
}

void WordBook::on_wordOmitBtn_clicked()
{
    WordModify *wm=new WordModify();
    wm->show();
     connect(this,SIGNAL(deleteWord(QString,int)),wm,SLOT(replyDeleteWord(QString,int)));
    emit deleteWord(txt,row);
}

void WordBook::on_tableWidget_clicked(const QModelIndex &index)
{
 //   qDebug()<<"clicked";
    this->row=ui->tableWidget->currentRow();
 //   qDebug()<<"row="<<ui->tableWidget->currentRow();
    ui->wordEditBtn->setEnabled(true);
    ui->wordOmitBtn->setEnabled(true);
}

void WordBook::on_wordSynBtn_clicked()
{
    replyShowWord(this->txt);
}

#include "logindialog.h"
#include "ui_logindialog.h"
#include<QTextStream>
LoginDialog::LoginDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setMouseTracking(false);
     setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);//设置主窗口无边框

}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
     {
        this->setMouseTracking(true);
          mouse_press = true;
          //鼠标相对于窗体的位置（或者使用event->globalPos() - this->pos()）
          move_point = event->pos();;
     }
}

void LoginDialog::mouseReleaseEvent(QMouseEvent *event)
{
    //设置鼠标为未被按下
        mouse_press = false;
}

void LoginDialog::mouseMoveEvent(QMouseEvent *event)
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



void LoginDialog::on_toolButton_clicked()
{
    this->close();
}

void LoginDialog::on_loginBtn_clicked()//点击登录按钮的情况
{
    QString inputUsername=ui->username->text().trimmed();
    QString inputPassword=ui->password->text().trimmed();
    QFile *wordFile =new QFile(inputUsername+".txt");
    wordFile->open(QFile::WriteOnly|QIODevice::Text|QIODevice::Append);

    wordFile->close();
    emit loginSignal(inputUsername,inputPassword);
}




void LoginDialog::replyConnectSuccess()
{
    ui->logintitle->setText("已连接网络，正验证用户信息..");
}

void LoginDialog::replyConnectFail()
{
    ui->logintitle->setText("无法连接到网络，请稍后再试!.");
}

void LoginDialog::replyVerifySuccess()
{
    ui->logintitle->setText("登录成功！");
    deleteLater();
}

void LoginDialog::replyVerifyFail()
{
    ui->logintitle->setText("用户或密码名错误！");
}

void LoginDialog::on_password_returnPressed()
{
    QString inputUsername=ui->username->text().trimmed();
    QString inputPassword=ui->password->text().trimmed();
    QFile *wordFile =new QFile(inputUsername+".txt");
    wordFile->open(QFile::WriteOnly|QIODevice::Text|QIODevice::Append);

    wordFile->close();
    emit loginSignal(inputUsername,inputPassword);
}

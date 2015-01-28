#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include"basicwindow.h"
#include<QDebug>
#include<QFile>
namespace Ui {
class LoginDialog;
}

class LoginDialog : public QWidget
{
    Q_OBJECT
    
public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
    QPoint move_point; //移动的距离
    bool mouse_press; //鼠标按下
    //鼠标按下事件
    void mousePressEvent(QMouseEvent *event);
   //鼠标释放事件
    void mouseReleaseEvent(QMouseEvent *event);
   //鼠标移动事件
    void mouseMoveEvent(QMouseEvent *event);
signals:
    void loginSignal(QString inputUsername,QString inputPassword);
private slots:
    void replyConnectSuccess();
    void replyConnectFail();
    void replyVerifySuccess();
    void replyVerifyFail();
    void on_toolButton_clicked();

    void on_loginBtn_clicked();

    void on_password_returnPressed();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H

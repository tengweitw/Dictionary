#ifndef WORDMODIFY_H
#define WORDMODIFY_H

#include <QWidget>
#include<QMouseEvent>
#include<QDebug>
namespace Ui {
class WordModify;
}

class WordModify : public QWidget
{
    Q_OBJECT
    
public:
    int flag;
    QString txtname;
    int row;
    explicit WordModify(QWidget *parent = 0);
    ~WordModify();
    QPoint move_point; //移动的距离
    bool mouse_press; //鼠标按下
    //鼠标按下事件
    void mousePressEvent(QMouseEvent *event);
   //鼠标释放事件
    void mouseReleaseEvent(QMouseEvent *event);
   //鼠标移动事件
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void on_wordCloseBtn_clicked();
    void replyAddWord(QString);
    void replyEditWord(QString,int);\
    void replyDeleteWord(QString,int);
    bool on_wordModifyBtn_clicked();

private:
    Ui::WordModify *ui;
};

#endif // WORDMODIFY_H

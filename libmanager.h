#ifndef LIBMANAGER_H
#define LIBMANAGER_H

#include <QWidget>

namespace Ui {
class LibManager;
}

class LibManager : public QWidget
{
    Q_OBJECT
    
public:
    int Flag;
    explicit LibManager(QWidget *parent = 0);
    ~LibManager();
    QPoint move_point; //移动的距离
    bool mouse_press; //鼠标按下
    //鼠标按下事件
    void mousePressEvent(QMouseEvent *event);
   //鼠标释放事件
    void mouseReleaseEvent(QMouseEvent *event);
   //鼠标移动事件
    void mouseMoveEvent(QMouseEvent *event);
private slots:
    void on_toolButton_clicked();

    void on_WeiKeiBtn_clicked();

    void on_ChineseBtn_clicked();

    void on_SimilarBtn_clicked();

    void on_ClanguageBtn_clicked();

    void on_HuaWeiBtn_clicked();

    void on_ProverbBtn_clicked();

    void on_PoemsBtn_clicked();

    void on_EngineerBtn_clicked();

    void on_quedingBtn_clicked();

    void replyWhichFlag(int);

private:
    Ui::LibManager *ui;
signals:
    void selectWhich(int Flag);
};

#endif // LIBMANAGER_H

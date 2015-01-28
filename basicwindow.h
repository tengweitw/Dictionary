#ifndef BASICWINDOW_H
#define BASICWINDOW_H
#include<QString>
#include<QWidget>
#include<QDebug>
#include<QMouseEvent>
#include<QDesktopServices>
#define MARGIN 10

namespace Ui {
class BasicWindow;
}

class BasicWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit BasicWindow(QWidget *parent = 0);
    ~BasicWindow();

    bool isLeftPressed;
    int curPos;
    QPoint pLast;

    int countFlag(QPoint p, int row);
    void setCursorType(int flag);
    int countRow(QPoint p);
private:
    Ui::BasicWindow *ui;
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
signals:
    void movesignal();
};

#endif // BASICWINDOW_H

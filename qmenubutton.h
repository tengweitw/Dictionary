#ifndef QMENUBUTTON_H
#define QMENUBUTTON_H

#include <QToolButton>
class QMenu;
class QMenuButton : public QToolButton
{
    Q_OBJECT
public:
    explicit QMenuButton(QWidget *parent = 0);
     QMenu * getmenu();
     QMenu * menu;
signals:
    
public slots:
    void popupmenu();
};

#endif // QMENUBUTTON_H

#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H
#include"basicwindow.h"
#include"network.h"
#include"logindialog.h"
#include"localdict.h"
#include"speech.h"
#include"wordbook.h"
#include"libmanager.h"
#include"qmenubutton.h"
#include<QSystemTrayIcon>
#include<QMenu>
#include<QCloseEvent>
#include<QAction>
#include<QEvent>
#include<ActiveQt/QAxObject>
#include<QtWebKit>
#include<QtWebKitWidgets/QWebView>
#include<QListWidgetItem>
#include<QMenu>
#include<QAction>
#include<QCompleter>
#include<QAbstractItemModel>
#include <QAbstractItemView>
#include <QModelIndex>
#include<QIcon>
#include<QPixmap>
#include<QTextStream>
#include<QStringList>
#include<QMessageBox>

namespace Ui {
class SearchWindow;
}

class SearchWindow : public BasicWindow
{
    Q_OBJECT
    
public:
    explicit SearchWindow(BasicWindow *parent = 0);
    ~SearchWindow();

    netWork network;
    bool isTop;
    bool isAdded;
    LocalDict *localDict;
    LibManager *libmag;
    QCompleter *completer;
    QList<QString> *history;
  //  QStringList slist;
    int p;
    int DictFlag;
    int skinFlag;

private slots:
    void on_netSearchBtn_clicked();

    void on_closeBtn_clicked();

    void on_minBtn_clicked();

    void on_toTopBtn_clicked();

    void on_searchBtn_clicked();

    void receiveQueryWord(Word *word);
    void on_lineEdit_returnPressed();

    void on_soundBtn_clicked();

    void closeEvent(QCloseEvent *event);
    void onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason);//系统托盘
    void exit();//系统托盘对应的与退出按钮对应的槽

    void replyLoginSignal(QString inputUsername,QString inputPassword);
    void replyVerifySuccessUserBtn();
    void on_loginBtn_toggled(bool checked);

    void on_netStudyBtn_clicked();

    void on_LocalSound_clicked();

    void on_wordBtn_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_backBtn_clicked();

    void on_forwardBtn_clicked();

    void smallSize();
    void midSize();
    void bigSize();
    void logout();
    void SetFirst();
    void SetMid();
    void About();
    void replySelectWhich(int Flag);
    void on_lexiconBtn_clicked();
    void replyNetState(bool);
    void on_skinBtn_clicked();

    void on_addButton_clicked();
    void Help();

private:
    Ui::SearchWindow *ui;
    QSystemTrayIcon *trayicon;//将系统托盘及菜单定义为窗口的私有成员
    QMenu *trayiconMenu;//系统托盘
signals:
    void exitsignal();//系统托盘发射程序退出信号
  //  void movesignal();
    void whichFlag(int);
    void SetCompleter(QString str);
    void ShowWord(QString);
};


#endif // SEARCHWINDOW_H

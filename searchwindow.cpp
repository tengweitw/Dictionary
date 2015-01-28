#include "searchwindow.h"
#include "ui_searchwindow.h"
#include<QDateTime>
#include<QDir>
SearchWindow::SearchWindow(BasicWindow *parent) :
    BasicWindow(parent),
    ui(new Ui::SearchWindow)
{
    ui->setupUi(this);
    this->setMouseTracking(true);
    ui->listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置ListWidget的边框虚线
    ui->listWidget->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit->setFocus();
    p=0;//指定前进和后退按钮位置
    isTop=false;//是否置顶标志
    isAdded=false;//是否已添加到单词本标志
    //slist.clear();//初始化自动补全的字符串列表
    skinFlag=1;//皮肤标志
    DictFlag=1;//初始化
    history = new QList<QString>();//存储历史查词记录
    localDict=new LocalDict();
    connect(&this->network,SIGNAL(signalQueryFinished(Word *)),this,SLOT(receiveQueryWord(Word *)));//查词完成操作
    this->network.connectNet();//连接网络
    //系统托盘操作
    trayicon=new QSystemTrayIcon(this);
    QAction *quit=new QAction(tr("退出"), this);
    QAction *help=new QAction(tr("关于"),this);
    help->setIcon(QIcon(":/searchwindow/searchwindow/help_hover.png"));
    quit->setIcon(QIcon(":/searchwindow/searchwindow/exit.png"));
    QIcon icon(":/searchwindow/searchwindow/trayicon.png");//注意修改这里的图片目录
    setWindowIcon(icon);

    trayiconMenu=new QMenu(this);
     trayiconMenu->addAction(help);
    trayiconMenu->addAction(quit);//这里可以添加多个菜单

   // trayiconMenu->addSeparator();
    trayicon->setIcon(icon);

    trayicon->setToolTip(tr("我爱查词典"));
    trayicon->setContextMenu(trayiconMenu);
    connect(quit,SIGNAL(triggered(bool)),this,SLOT(exit()));
    connect(help,SIGNAL(triggered(bool)),this,SLOT(Help()));
    connect(trayicon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason)));
    trayicon->show();


    //隐藏操作
    ui->addButton->hide();
    ui->soundBtn->hide();
    ui->localAdd->hide();
    ui->LocalSound->hide();
    ui->PronLabel->hide();
    //主菜单操作

       QMenu * mainMenu = ui->mainMenuBtn->getmenu();//获得菜单，并向上面添加菜单
       QAction* ToTop = mainMenu->addAction("置顶");
       ToTop->setIcon(QIcon(":/searchwindow/searchwindow/topon_normal.png"));
       connect(ToTop, SIGNAL(triggered()), this, SLOT(on_toTopBtn_clicked()));

       QMenu *fontMenu=mainMenu->addMenu("字体大小");
       fontMenu->setIcon(QIcon(":/searchwindow/searchwindow/fontsize_normal.png"));

       QAction* FontSize1 = fontMenu->addAction("小");
       FontSize1->setIcon(QIcon(":/searchwindow/searchwindow/Aminus_normal.png"));
       connect(FontSize1, SIGNAL(triggered()), this, SLOT(smallSize()));

       QAction* FontSize2 = fontMenu->addAction("中");
       FontSize2->setIcon(QIcon(":/searchwindow/searchwindow/A_normal.png"));
       connect(FontSize2, SIGNAL(triggered()), this, SLOT(midSize()));

       QAction* FontSize3 = fontMenu->addAction("大");
       FontSize3->setIcon(QIcon(":/searchwindow/searchwindow/aplus_normal.png"));
       connect(FontSize3, SIGNAL(triggered()), this, SLOT(bigSize()));


       QMenu *SearchMenu=mainMenu->addMenu("搜索");
       SearchMenu->setIcon(QIcon(":/searchwindow/searchwindow/settings_normal.png"));

       QAction* SearchFirst = SearchMenu->addAction("句首匹配");
     //  Setting->setIcon(QIcon(":/searchwindow/searchwindow/settings_normal.png"));
       connect(SearchFirst, SIGNAL(triggered()), this, SLOT(SetFirst()));

       QAction* SearchMid = SearchMenu->addAction("句中匹配");
     //  Setting->setIcon(QIcon(":/searchwindow/searchwindow/settings_normal.png"));
       connect(SearchMid, SIGNAL(triggered()), this, SLOT(SetMid()));

       QAction* Help = mainMenu->addAction("关于");
       Help->setIcon(QIcon(":/searchwindow/searchwindow/help_normal.png"));
       connect(Help, SIGNAL(triggered()), this, SLOT(About()));

       QAction* LogOut = mainMenu->addAction("注销");
       LogOut->setIcon(QIcon(":/searchwindow/searchwindow/logout_normal.png"));
       connect(LogOut, SIGNAL(triggered()), this, SLOT(logout()));

       QAction* Exit = mainMenu->addAction("退出");
       Exit->setIcon(QIcon(":/searchwindow/searchwindow/Exit_normal.png"));
       connect(Exit, SIGNAL(triggered()), this, SLOT(exit()));
       //自动填充功能


       //slist= this->localDict->loadDictWeiKe();//导入词条
       ui->lineEdit->words=this->localDict->loadDictWeiKe();//导入词条

       connect(this,SIGNAL(movesignal()),ui->lineEdit,SLOT(replyMove()));
       connect(&this->network,SIGNAL(NetState(bool)),this,SLOT(replyNetState(bool)));
       ui->stateBtn->setText("正在使用维基英汉词典");
   //     qDebug()<<"sfdddddffffffffff";
       ui->userinfBtn->setText("我爱查词典");
       connect(this,SIGNAL(SetCompleter(QString)),ui->lineEdit,SLOT(setCompleter(QString)));

       ui->addButton->setStyleSheet("QToolButton{border-image: url(:/searchwindow/searchwindow/add_normal.png);}"
                                    "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/add_hover.png);}");
}

SearchWindow::~SearchWindow()
{
    delete ui;
}



void SearchWindow::on_netSearchBtn_clicked()//网页查词
{
    QDesktopServices::openUrl(QUrl("http://www.baidu.com/s?wd="+ui->lineEdit->text().trimmed()));
}

void SearchWindow::on_closeBtn_clicked()
{
    this->hide();
}

void SearchWindow::on_minBtn_clicked()
{
    setWindowState(Qt::WindowMinimized);
}



void SearchWindow::on_toTopBtn_clicked()//置顶按钮操作
{
    isTop=!isTop;
  //  qDebug()<<isTop;
    if(isTop)//将窗口置顶
    {
        this->hide();
   //     qDebug()<<"flag="<<this->windowFlags();
        this->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    //     qDebug()<<"flag1="<<this->windowFlags();
        this->show();
         ui->toTopBtn->setStyleSheet("QToolButton{border-image: url(:/searchwindow/searchwindow/topon_normal.png);}"
                                     "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/topon_hover.png);}");
    //     qDebug()<<"取消置顶";
         ui->toTopBtn->setToolTip(tr("取消置顶"));

    }
    else
    {
        this->hide();

        this->setWindowFlags(this->windowFlags()^Qt::WindowStaysOnTopHint|Qt::WindowStaysOnBottomHint);
    //     qDebug()<<"flag2="<<this->windowFlags();
        this->show();
         ui->toTopBtn->setStyleSheet("QToolButton{border-image: url(:/searchwindow/searchwindow/topoff_normal.png);}"
                                     "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/topoff_hover.png);}");
         ui->toTopBtn->setToolTip(tr("置顶"));
    }
}

void SearchWindow::on_searchBtn_clicked() //查词按钮操作
{

    QString yourword=ui->lineEdit->text().trimmed();
    if(!yourword.isEmpty())
    {
        history->append(yourword);
        p=history->size()-1;
   //     qDebug()<<yourword;
        localDict->strPron.clear();
        this->network.queryWord(yourword);
       // localDict->loadDictWeiKe();
      //  localDict->loadDictChinese();
      //  qDebug()<<"sssssssssssss";
        if(DictFlag==1)
        {
            if(localDict->searchWordWeiKe(yourword)=="")
            {
                ui->LocaltextBrowser->setText(tr("本地词库查无此词，请使用网络查词"));
                ui->localWordName->clear();
                ui->localPron->clear();
                ui->localAdd->hide();
                ui->LocalSound->hide();
                ui->localWordName->setText(yourword);
        //        qDebug()<<"aaaaaaaaaaaaa";
            }
            else
            {
                ui->LocaltextBrowser->setText(localDict->searchWordWeiKe(yourword));

                ui->localWordName->setText(yourword);
                ui->localPron->setText(localDict->strPron);

            }
        }
        else
        {
            if(localDict->searchWord(yourword,ui->lineEdit->words,DictFlag)=="")
            {
                ui->LocaltextBrowser->setText(tr("本地词库查无此词，请使用网络查词"));
                ui->localWordName->clear();
                ui->localPron->clear();
                ui->localAdd->hide();
                ui->LocalSound->hide();
                ui->localWordName->setText(yourword);
            }
            else
            {
                ui->LocaltextBrowser->setText(localDict->searchWord(yourword,ui->lineEdit->words,DictFlag));
                ui->localAdd->hide();
                ui->localWordName->setText(yourword);
                ui->localPron->setText(localDict->strPron);

            }
        }
    }
    ui->lineEdit->setFocus();

}

void SearchWindow::receiveQueryWord(Word *yourword)
{

    if(yourword->pron=="")
        ui->PronLabel->hide();
    else
        ui->PronLabel->show();
  //  qDebug()<<yourword->audio;
    QString str1=yourword->name;
    QString str2="<br></font>"+yourword->definition+"<br>"+yourword->en_definitions;
    QString str3="[<font face=\"Times New Roman\"> "+yourword->pron+"</font>]";

    ui->wordName->setText(str1);//要查的单词
    ui->textBrowser->setText(str2);//单词 英译 中译
    ui->PronLabel->setText(str3);//单词音标
    if(yourword->definition!="")
    {
        ui->addButton->show();
        ui->soundBtn->show();

    }
    else
    {
        ui->addButton->hide();
        ui->soundBtn->hide();
         ui->textBrowser->setText(tr("查无此词，请使用本地词典或网络搜索"));
    }



    ui->localAdd->show();
    ui->LocalSound->show();
 //   ui->textBrowser->setText(yourword->definition);

}

void SearchWindow::on_lineEdit_returnPressed()//回车操作
{
    QString yourword=ui->lineEdit->text().trimmed();
    if(!yourword.isEmpty())
    {
        QString wordbookName;
        QFile *readFile =new QFile(this->network.username+".txt");
        readFile->open(QFile::ReadWrite|QIODevice::Text);
        QTextStream in(readFile);
        ui->addButton->setStyleSheet("QToolButton{border-image: url(:/searchwindow/searchwindow/add_normal.png);}"
                                     "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/add_hover.png);}");
        isAdded=false;
        while(!in.atEnd())
        {
             wordbookName=in.readLine();

             if(wordbookName.trimmed()==yourword)
             {

               isAdded=true;
               ui->addButton->setStyleSheet("QToolButton{border-image: url(:/searchwindow/searchwindow/study_normal.png);}"
                                            "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/studied_hover.png);}");
               break;
             }
        }
        readFile->close();

        history->append(yourword);
        p=history->size()-1;

        localDict->strPron.clear();
        this->network.queryWord(yourword);
       // localDict->loadDictWeiKe();
      //  localDict->loadDictChinese();
      //  qDebug()<<"sssssssssssss";
        if(DictFlag==1)
        {
            if(localDict->searchWordWeiKe(yourword)=="")
            {
                ui->LocaltextBrowser->setText(tr("本地词库查无此词，请使用网络查词"));
                ui->localWordName->clear();
                ui->localPron->clear();
                ui->localAdd->hide();
                ui->LocalSound->hide();
                ui->localWordName->setText(yourword);

            }
            else
            {
                ui->LocaltextBrowser->setText(localDict->searchWordWeiKe(yourword));

                ui->localWordName->setText(yourword);
                ui->localPron->setText(localDict->strPron);

            }
        }
        else
        {
            if(localDict->searchWord(yourword,ui->lineEdit->words,DictFlag)=="")
            {
                ui->LocaltextBrowser->setText(tr("本地词库查无此词，请使用网络查词"));
                ui->localWordName->clear();
                ui->localPron->clear();
                ui->localAdd->hide();
                ui->LocalSound->hide();
                ui->localWordName->setText(yourword);
            }
            else
            {
                ui->LocaltextBrowser->setText(localDict->searchWord(yourword,ui->lineEdit->words,DictFlag));
                ui->localAdd->hide();
                ui->localWordName->setText(yourword);
                ui->localPron->setText(localDict->strPron);

            }
        }
    }
    ui->lineEdit->setFocus();
}

void SearchWindow::on_soundBtn_clicked()//发音按钮
{
    QString yourword=ui->lineEdit->text().trimmed();
   // this->network.gotword->play("hello");
   // qDebug()<<this->network.gotword->audio;
    QMediaPlayer *player= new QMediaPlayer();
    player->setMedia( QUrl(this->network.gotword->audio));
    player->play();
}

void SearchWindow::closeEvent(QCloseEvent *event)
{
    if(trayicon->isVisible())
    {
        hide();
        this->close();
        event->ignore();
    }
}

void SearchWindow::onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason)//系统托盘按钮操作
{
    switch(reason)
      {
      //单击

      case QSystemTrayIcon::Trigger:
        //  qDebug()<<this->isEnabled();
      //双击

     //     flag=!flag;
     // case QSystemTrayIcon::DoubleClick:
          //恢复窗口显示
          if(this->isVisible())//判断窗口是否可见
          {
              if(this->isMinimized())//窗口是否是最小化
                {
                  //  this->setWindowFlags(this->windowFlags()|Qt::WindowStaysOnTopHint);//置顶
                //  this->setWindowFlags(Qt::Widget);//取消置顶
                    this->setWindowState(Qt::WindowActive);
                    this->setGeometry(this->geometry());//使得每次显示窗口为上次的位置
                    this->show();
                }
              else
                    this->hide();
          }
           else
          {
             // this->setWindowFlags(this->windowFlags()|Qt::WindowStaysOnTopHint);//置顶
                //  this->setWindowFlags(Qt::Widget);//取消置顶
                    this->setWindowState(Qt::WindowActive);
                    this->setGeometry(this->geometry());
                    this->show();
          }
          break;
      default:
          break;
      }
}

void SearchWindow::exit()
{
    emit exitsignal();
}



void SearchWindow::replyLoginSignal(QString inputUsername, QString inputPassword)
{
  //  qDebug()<<"用户正在登录……";
    this->network.connectNet(inputUsername,inputPassword);

}

void SearchWindow::replyVerifySuccessUserBtn()
{

    if(this->network.username!="nineheadedbird")
       {
        ui->loginBtn->setStyleSheet("QToolButton{border-image: url(:/searchwindow/searchwindow/loginon_normal.png);}"
                                    "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/loginon_hover.png);}");
        ui->loginBtn->setToolTip("注销");
         ui->userinfBtn->setText(this->network.username+"--Online");
    }

}



void SearchWindow::on_loginBtn_toggled(bool checked)//注意主窗口关闭，这个未关闭
{//这里可以用flag来判断，比目前这个好一点
    if(checked)
    {
    LoginDialog *loginDialog=new LoginDialog();
    loginDialog->show();
    connect(loginDialog,SIGNAL(loginSignal(QString,QString)),this,SLOT(replyLoginSignal(QString,QString)));
    connect(&(this->network),SIGNAL(connectSuccess()),loginDialog,SLOT(replyConnectSuccess()));
    connect(&(this->network),SIGNAL(connectFail()),loginDialog,SLOT(replyConnectFail()));
    connect(&(this->network),SIGNAL(verifySuccess()),loginDialog,SLOT(replyVerifySuccess()));
    connect(&(this->network),SIGNAL(verifyFail()),loginDialog,SLOT(replyVerifyFail()));
    connect(&(this->network),SIGNAL(verifySuccess()),this,SLOT(replyVerifySuccessUserBtn()));
    }
    else
    {
      //  qDebug()<<"注销";
        ui->loginBtn->setStyleSheet("QToolButton{border-image: url(:/searchwindow/searchwindow/login_normal.png);}"
                                    "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/login_hover.png);}");
        ui->loginBtn->setToolTip("登录");
        this->network.username="nineheadedbird";
        this->network.connectNet();
         ui->userinfBtn->setText("我爱查词典");
    }
}

void SearchWindow::on_netStudyBtn_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.shanbay.com"));
}

void SearchWindow::on_LocalSound_clicked()
{
    QAxObject *speech= new QAxObject(this);
    speech->setControl("SAPI.SpVoice");
 //   qDebug()<<"speak";

    speech->dynamicCall("Speak(QString,uint)",ui->localWordName->text(),1);
}

void SearchWindow::on_wordBtn_clicked()//点击词库管理按钮
{
    WordBook *wordBook = new WordBook();
   connect(this,SIGNAL(ShowWord(QString)),wordBook,SLOT(replyShowWord(QString)));

    emit ShowWord(this->network.username);


    wordBook->show();

}

void SearchWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    if(item->text()==QString("网络搜索"))
    {
       ui->webView->load(QUrl("http://www.baidu.com/s?wd="+ui->lineEdit->text().trimmed()));
    }
}


void SearchWindow::on_backBtn_clicked()
{

    if(p!=0)
    {
         p--;

        QString yourword=history->at(p);
        ui->lineEdit->setText(yourword);
        this->network.queryWord(yourword);
        localDict->loadDictWeiKe();

        if(localDict->searchWordWeiKe(yourword)=="")
        {
            ui->LocaltextBrowser->setText(tr("本地词库查无此词，请使用网络查词"));
            ui->localAdd->hide();
            ui->LocalSound->hide();
        }
        else
        {
            ui->LocaltextBrowser->setText(localDict->searchWordWeiKe(yourword));
            ui->localWordName->setText(yourword);
            ui->localPron->setText(localDict->strPron);

        }
    }

}

void SearchWindow::on_forwardBtn_clicked()
{
    if(p!=history->size()-1)
    {
         p++;

        QString yourword=history->at(p);
        ui->lineEdit->setText(yourword);
        this->network.queryWord(yourword);
        localDict->loadDictWeiKe();

        if(localDict->searchWordWeiKe(yourword)=="")
        {
            ui->LocaltextBrowser->setText(tr("本地词库查无此词，请使用网络查词"));
            ui->localAdd->hide();
            ui->LocalSound->hide();
        }
        else
        {
            ui->LocaltextBrowser->setText(localDict->searchWordWeiKe(yourword));
            ui->localWordName->setText(yourword);
            ui->localPron->setText(localDict->strPron);

        }
    }
}

void SearchWindow::smallSize()
{
    ui->textBrowser->setStyleSheet("font:SimSun-ExtB;font-size : 16px");
    ui->LocaltextBrowser->setStyleSheet("font:SimSun-ExtB;font-size : 16px");
    ui->webView->setStyleSheet("font-size : 16px");
}

void SearchWindow::midSize()
{
    ui->textBrowser->setStyleSheet("font:SimSun-ExtB;font-size : 18px");
    ui->LocaltextBrowser->setStyleSheet("font:SimSun-ExtB;font-size : 18px");
     ui->webView->setStyleSheet("font-size : 18px");
}

void SearchWindow::bigSize()
{
    ui->textBrowser->setStyleSheet("font:SimSun-ExtB;font-size : 20px");
    ui->LocaltextBrowser->setStyleSheet("font:SimSun-ExtB;font-size : 20px");
     ui->webView->setStyleSheet("font-size : 20px");
}

void SearchWindow::logout()
{
    ui->loginBtn->setStyleSheet("QToolButton{border-image: url(:/searchwindow/searchwindow/login_normal.png);}"
                            "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/login_hover.png);}");
    ui->loginBtn->setToolTip("登录");
    this->network.connectNet();
    ui->loginBtn->setChecked(false);
    ui->userinfBtn->setText("");
}

void SearchWindow::SetFirst()
{
    ui->lineEdit->ModelFlag=1;
    emit SetCompleter(ui->lineEdit->text());

}

void SearchWindow::SetMid()
{
    ui->lineEdit->ModelFlag=0;\
    emit SetCompleter(ui->lineEdit->text());

}



void SearchWindow::About()
{

    QMessageBox::about(this, tr("关于"), tr("This dictionary is designed by TW. "
                                          "Thank you for using! Please feel free to contact me for any questions via email(771257840@qq.com)."));
}


void SearchWindow::replySelectWhich(int Flag)
{
 //   qDebug()<<"replyyyyyyyyyyy";
    this->DictFlag=Flag;
    if(Flag==1)
    {

      //  this->localDict->loadDictWeiKe().clear();
        ui->lineEdit->words.clear();
       // this->slist=this->localDict->loadDictWeiKe();
        ui->lineEdit->words=this->localDict->loadDictWeiKe();

    }
    else
    {
        // this->localDict->loadDictWeiKe().clear();
        //this->slist=this->localDict->loadDict(Flag);
        ui->lineEdit->words=this->localDict->loadDict(Flag);

    }

    switch(Flag)
    {
    case 1:ui->stateBtn->setText("正在使用维基英汉词典");
        break;
    case 2:ui->stateBtn->setText("正在使用C语言库函数");
        break;
    case 3:ui->stateBtn->setText("正在使用英语同义词辨析");
        break;
    case 4:ui->stateBtn->setText("正在使用华为网络通信术语");
        break;
    case 5:ui->stateBtn->setText("正在使用英语谚语");
        break;
    case 6:ui->stateBtn->setText("正在使用唐诗300首");
        break;
    case 7:ui->stateBtn->setText("正在使用电子工程词典");
        break;
    case 8:ui->stateBtn->setText("正在使用中华大辞典");
        break;

    }
}



void SearchWindow::on_lexiconBtn_clicked()
{
    this->libmag=new LibManager();
    connect(this->libmag,SIGNAL(selectWhich(int)),this,SLOT(replySelectWhich(int)));
    connect(this,SIGNAL(whichFlag(int)),this->libmag,SLOT(replyWhichFlag(int)));
    libmag->show();
    emit whichFlag(this->DictFlag);
}

void SearchWindow::replyNetState(bool flag)
{
    if(flag)
    {
        ui->netStateBtn->setToolTip("网络已连接");
        ui->netStateBtn->setStyleSheet("QToolButton{border-image: url(:/searchwindow/searchwindow/neton_normal.png);}"
                                        "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/neton_hover.png);}");
    }
    else
    {
        ui->netStateBtn->setToolTip("网络未连接");
        ui->netStateBtn->setStyleSheet("QToolButton{border-image: url(:/searchwindow/searchwindow/nonet_normal.png);}"
                                        "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/nonet_hover.png);}");
    }
}

void SearchWindow::on_skinBtn_clicked()
{
    /******************经典蓝*****************************/
    skinFlag=++skinFlag%6;
    if(skinFlag==0)
    {
    ui->widget->setStyleSheet("background-color: rgb(98, 93, 255);");
    ui->netSearchBtn->setStyleSheet("QToolButton{background-color: rgb(98, 93, 255);border-image: url(:/searchwindow/searchwindow/netsearch_normal.png);}"
                                    "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/netsearch_hover.png);}");
    ui->forwardBtn->setStyleSheet("QToolButton{background-color: rgb(98, 93, 255);border-image: url(:/searchwindow/searchwindow/forward_normal.png);}"
                                  "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/forward_hover.png);}");
    ui->backBtn->setStyleSheet("QToolButton{background-color: rgb(98, 93, 255);border-image: url(:/searchwindow/searchwindow/back_normal.png);}"
                               "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/back_hover.png);}");
    ui->searchBtn->setStyleSheet("QToolButton{background-color: rgb(98, 93, 255);border-image: url(:/searchwindow/searchwindow/search.png);}"
                                 "QToolButton:hover{background-color: rgba(98, 93, 255,100);}");
    ui->listWidget->setStyleSheet("QListWidget::item{width:40px;height:40px;font:bold 20px;}"
                                  "QListWidget {background-color: rgb(255, 255, 255);}"
                                  "QListWidget::item:selected:!active {background-color: rgb(98, 93, 255);}"
                                  "QListWidget::item:selected:active {background-color: rgb(98, 93, 255);}"
                                  "QListWidget::item:hover {background-color:rgba(50, 23, 255, 100);}");
    ui->widget_2->setStyleSheet("background-color: rgb(98, 93, 255);");
    }
    else if(skinFlag==1)
    {
    /******************墨绿色*****************************/
    ui->widget->setStyleSheet("background-color: rgb(20, 93, 25);");
    ui->netSearchBtn->setStyleSheet("QToolButton{background-color: rgb(20, 93, 25);border-image: url(:/searchwindow/searchwindow/netsearch_normal.png);}"
                                    "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/netsearch_hover.png);}");
    ui->forwardBtn->setStyleSheet("QToolButton{background-color: rgb(20, 93, 25);border-image: url(:/searchwindow/searchwindow/forward_normal.png);}"
                                  "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/forward_hover.png);}");
    ui->backBtn->setStyleSheet("QToolButton{background-color: rgb(20, 93, 25);border-image: url(:/searchwindow/searchwindow/back_normal.png);}"
                               "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/back_hover.png);}");
    ui->searchBtn->setStyleSheet("QToolButton{background-color: rgb(20, 93, 25);border-image: url(:/searchwindow/searchwindow/search.png);}"
                                 "QToolButton:hover{background-color: rgb(20, 93, 25,100);}");
    ui->listWidget->setStyleSheet("QListWidget::item{width:40px;height:40px;font:bold 20px;}"
                                  "QListWidget {background-color: rgb(255, 255, 255);}"
                                  "QListWidget::item:selected:!active {background-color: rgb(20, 93, 25);}"
                                  "QListWidget::item:selected:active {background-color: rgb(98, 93, 255);}"
                                  "QListWidget::item:hover {background-color: rgb(0, 239, 0, 100);}");
    ui->widget_2->setStyleSheet("background-color: rgb(20, 93, 25);");
    }
    else if(skinFlag==2)
    {
    /******************粉红色*****************************/
    ui->widget->setStyleSheet("	background-color: rgb(255, 65, 198);");
    ui->netSearchBtn->setStyleSheet("QToolButton{	background-color: rgb(255, 65, 198);border-image: url(:/searchwindow/searchwindow/netsearch_normal.png);}"
                                    "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/netsearch_hover.png);}");
    ui->forwardBtn->setStyleSheet("QToolButton{	background-color: rgb(255, 65, 198);border-image: url(:/searchwindow/searchwindow/forward_normal.png);}"
                                  "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/forward_hover.png);}");
    ui->backBtn->setStyleSheet("QToolButton{	background-color: rgb(255, 65, 198);border-image: url(:/searchwindow/searchwindow/back_normal.png);}"
                               "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/back_hover.png);}");
    ui->searchBtn->setStyleSheet("QToolButton{	background-color: rgb(255, 65, 198);border-image: url(:/searchwindow/searchwindow/search.png);}"
                                 "QToolButton:hover{background-color: rgba(255, 65, 198,100);}");
    ui->listWidget->setStyleSheet("QListWidget::item{width:40px;height:40px;font:bold 20px;}"
                                  "QListWidget {background-color: rgb(255, 255, 255);}"
                                  "QListWidget::item:selected:!active {	background-color: rgb(255, 65, 198);}"
                                  "QListWidget::item:selected:active {background-color: rgb(98, 93, 255);}"
                                  "QListWidget::item:hover {background-color: rgb(255, 65, 198, 100);}");
    ui->widget_2->setStyleSheet("	background-color: rgb(255, 65, 198);");
    }
    else if(skinFlag==3)
    {
    ui->widget->setStyleSheet("background-color: rgb(240, 188, 89);");
    ui->netSearchBtn->setStyleSheet("QToolButton{background-color: rgb(240, 188, 89);border-image: url(:/searchwindow/searchwindow/netsearch_normal.png);}"
                                    "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/netsearch_hover.png);}");
    ui->forwardBtn->setStyleSheet("QToolButton{background-color: rgb(240, 188, 89);border-image: url(:/searchwindow/searchwindow/forward_normal.png);}"
                                  "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/forward_hover.png);}");
    ui->backBtn->setStyleSheet("QToolButton{background-color: rgb(240, 188, 89);border-image: url(:/searchwindow/searchwindow/back_normal.png);}"
                               "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/back_hover.png);}");
    ui->searchBtn->setStyleSheet("QToolButton{background-color: rgb(240, 188, 89);border-image: url(:/searchwindow/searchwindow/search.png);}"
                                 "QToolButton:hover{background-color: rgba(240, 188, 89);}");
    ui->listWidget->setStyleSheet("QListWidget::item{width:40px;height:40px;font:bold 20px;}"
                                  "QListWidget {background-color: rgb(255, 255, 255);}"
                                  "QListWidget::item:selected:!active {background-color: rgb(240, 188, 89);}"
                                  "QListWidget::item:selected:active {background-color: rgb(98, 93, 255);}"
                                  "QListWidget::item:hover {background-color:rgba(240, 188, 89, 100);}");
    ui->widget_2->setStyleSheet("background-color: rgb(240, 188, 89);");
    }
    else if(skinFlag==4)
    {
    ui->widget->setStyleSheet("background-color: rgb(177, 99, 159);");
    ui->netSearchBtn->setStyleSheet("QToolButton{background-color: rgb(177, 99, 159);border-image: url(:/searchwindow/searchwindow/netsearch_normal.png);}"
                                    "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/netsearch_hover.png);}");
    ui->forwardBtn->setStyleSheet("QToolButton{background-color: rgb(177, 99, 159);border-image: url(:/searchwindow/searchwindow/forward_normal.png);}"
                                  "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/forward_hover.png);}");
    ui->backBtn->setStyleSheet("QToolButton{background-color: rgb(177, 99, 159);border-image: url(:/searchwindow/searchwindow/back_normal.png);}"
                               "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/back_hover.png);}");
    ui->searchBtn->setStyleSheet("QToolButton{background-color: rgb(177, 99, 159);border-image: url(:/searchwindow/searchwindow/search.png);}"
                                 "QToolButton:hover{background-color: rgba(177, 99, 159);}");
    ui->listWidget->setStyleSheet("QListWidget::item{width:40px;height:40px;font:bold 20px;}"
                                  "QListWidget {background-color: rgb(255, 255, 255);}"
                                  "QListWidget::item:selected:!active {background-color: rgb(177, 99, 159);}"
                                  "QListWidget::item:selected:active {background-color: rgb(98, 93, 255);}"
                                  "QListWidget::item:hover {background-color:rgba(177, 99, 159, 100);}");
    ui->widget_2->setStyleSheet("background-color: rgb(177, 99, 159);");
    }
    else if(skinFlag==5)
    {
    ui->widget->setStyleSheet("background-color: rgb(49, 166, 107);");
    ui->netSearchBtn->setStyleSheet("QToolButton{background-color: rgb(49, 166, 107);border-image: url(:/searchwindow/searchwindow/netsearch_normal.png);}"
                                    "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/netsearch_hover.png);}");
    ui->forwardBtn->setStyleSheet("QToolButton{background-color: rgb(49, 166, 107);border-image: url(:/searchwindow/searchwindow/forward_normal.png);}"
                                  "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/forward_hover.png);}");
    ui->backBtn->setStyleSheet("QToolButton{background-color: rgb(49, 166, 107);border-image: url(:/searchwindow/searchwindow/back_normal.png);}"
                               "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/back_hover.png);}");
    ui->searchBtn->setStyleSheet("QToolButton{background-color: rgb(49, 166, 107);border-image: url(:/searchwindow/searchwindow/search.png);}"
                                 "QToolButton:hover{background-color: rgba(49, 166, 107);}");
    ui->listWidget->setStyleSheet("QListWidget::item{width:40px;height:40px;font:bold 20px;}"
                                  "QListWidget {background-color: rgb(255, 255, 255);}"
                                  "QListWidget::item:selected:!active {background-color: rgb(49, 166, 107);}"
                                  "QListWidget::item:selected:active {background-color: rgb(98, 93, 255);}"
                                  "QListWidget::item:hover {background-color:rgba(49, 166, 107, 100);}");
    ui->widget_2->setStyleSheet("background-color: rgb(49, 166, 107);");
    }
}

void SearchWindow::on_addButton_clicked()
{
    QString wordbookName;

  //  qDebug()<<"isAdded="<<isAdded;
    if(!isAdded)//新单词
    {
    //    qDebug()<<"新单词";
        isAdded=true;
        ui->addButton->setStyleSheet("QToolButton{border-image: url(:/searchwindow/searchwindow/study_normal.png);}"
                                     "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/studied_hover.png);}");
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date = current_date_time.toString("yyyy-MM-dd");
      //  qDebug()<<current_date;
        QFile *wordFile =new QFile(this->network.username+".txt");
        wordFile->open(QFile::ReadWrite|QIODevice::Text|QIODevice::Append);

        QTextStream out(wordFile);
         out.setCodec("utf-8");
        out<<ui->lineEdit->text()<<endl;
        out<<"["+this->network.gotword->pron+"]"<<endl;
        out<<this->network.gotword->definition<<endl;
        out<<current_date<<endl;//添加时间
        out<<endl;//完成度

        wordFile->close();

    }
    else//已经存在的单词
    {
   //     qDebug()<<"删除单词";
        ui->addButton->setStyleSheet("QToolButton{border-image: url(:/searchwindow/searchwindow/add_normal.png);}"
                                     "QToolButton:hover{border-image: url(:/searchwindow/searchwindow/add_hover.png);}");
        isAdded=false;
        QFile *readFile =new QFile(this->network.username+".txt");
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
             wordbookName=gbk_string;

             if(wordbookName.trimmed()==ui->wordName->text())
             {
                 while(!gbk_codec->toUnicode(readFile->readLine().trimmed()).contains("20"));
                 readFile->readLine();
                 wordbookName=gbk_codec->toUnicode(readFile->readLine().trimmed());
             }
             out.setCodec("utf-8");
             out<<wordbookName<<endl;
        }
        readFile->close();
        outFile->close();
        //删除原文件，重命名新文件
        QFile::remove(this->network.username+".txt");
        QFile::rename("temp.txt",this->network.username+".txt");


    }
}

void SearchWindow::Help()
{
    QMessageBox::about(this,tr("帮助"), tr(
                                         "----------------------------------版权声明----------------------------------\n"
                                         "1.网络查词部分是依据扇贝网公开的API进行设计的\n"
                                         "2.完全遵循LGPL协议,公开源代码。未经作者允许不得作为商业用途。\n"
                                         "3.软件名称：我爱查词典\n"
                                         "4.软件版本：1.0\n"
                                         "5.软件作者：nineheadedbird\n"
                                         "6.邮箱：771257840@qq.com\n"
                                         "7.Copyright(C) 2014-2015 Teng Wei rights reserved.\n"

                                         "----------------------------------软件优点----------------------------------\n"
                                         "1.本软件包括三种查词方式：网络查词、本地查词、网页查词\n"
                                         "2.本地查词中，默认有8本不同类型的词典，如有需要，可以通过邮件与我联系，添加其它词典\n"
                                         "3.本查词软件的自动补全分为两种：句首匹配补全、居中匹配补全\n"
                                         "4.采用了本地发音，即使在没有网络的情况下，也可以发音（含中文发音），win7平台\n"
                                         "5.系统托盘可以单击显示或者关闭，比较方便\n"
                                         "6.可以换肤，更换为自己喜欢的风格\n"
                                         "7.带有查词历史，可以查看以往查过的单词\n"
                                         "8.界面简洁美观\n"
                                         "----------------------------------软件不足----------------------------------\n"
                                         "1.软件占用内存大，有待优化\n"
                                         "2.屏幕取词尚未开发\n"
                                         "3.单词记忆界面尚未开发\n"
                                         "4.由于未经多次测试，可能存在bug，望使用者与我联系\n"
                                         "5.由于时间精力有限，上述不足之处敬请谅解，待日后有时间再予以解决\n"));
}


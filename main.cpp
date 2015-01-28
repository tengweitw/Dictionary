#include "searchwindow.h"
#include"logindialog.h"
#include"network.h"
#include"localdict.h"
#include"wordbook.h"
#include"libmanager.h"
#include"wordmodify.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

     SearchWindow sw;
     sw.show();
     QObject::connect(&sw,SIGNAL(exitsignal()),&a,SLOT(quit()));//将退出按钮与退出函数绑定

//    LoginDialog lw;
//    lw.show();

//        LocalDict ld;
//        ld.loadDictChinese();
//        qDebug()<<"aaaaaaaaaaaaa"<<ld.wordList[0];
//        qDebug()<<"aaaaaaaaaaaaa"<<ld.wordList[1];
//        qDebug()<<"aaaaaaaaaaaaa"<<ld.wordList[30001];
      //  qDebug()<<"bbbbbbbbbbbbb="<<ld.wordList[1].indexOf("<");
    //    ld.searchWord("hello");

//    network.loginShanbay();
//    network.queryWord("hello");

//    QAxObject speech;
//    speech.setControl("SAPI.SpVoice");
//    speech.dynamicCall("Speak(QString,uint)","你好",1);

//    WordBook *wordBook = new WordBook();
//    wordBook->show();

//       LibManager lm;
//        lm.show();

//      WordModify wm;
//      wm.show();
    return a.exec();
}

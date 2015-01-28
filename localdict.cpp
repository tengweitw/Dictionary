#include "localdict.h"
#include<QByteArray>
#include<QTextCodec>
LocalDict::LocalDict()
{
    this->strPron="";
    this->wordList.clear();
}

QStringList LocalDict::loadDictWeiKe()
{
    inFile=new QFile ("WeiKe.txt");
    in=new QTextStream (inFile);
    if(!inFile->open(QIODevice::ReadOnly))
    {
     //   qDebug()<<"cannot read!";

    }

    while(!in->atEnd())
    {

        if(in->readLine().trimmed()=="</>")
           wordList<<in->readLine();

    }
    inFile->close();
    return wordList;
}

QString LocalDict::searchWordWeiKe(QString inputWord)
{
    QString indexword;
    QString strtemp;
    QString str;

    inFile=new QFile ("WeiKe.txt");
    in=new QTextStream (inFile);
    if(!inFile->open(QIODevice::ReadOnly))
    {
   //     qDebug()<<"cannot read!";

    }
    indexword=in->readLine().trimmed();
  //  qDebug()<<indexword;

    while(QString::compare(indexword,inputWord,Qt::CaseInsensitive))
      {
          if(in->atEnd())
              return "";
          if(in->readLine().trimmed()=="</>")
              indexword=in->readLine().trimmed();
      }
      this->strPron=in->readLine().trimmed();
      this->strPron=in->readLine().trimmed();
       this->strPron.replace("<br>","");
    if(this->strPron.at(0)!='[')
    {
        str=str.append(this->strPron);
        this->strPron="";
    }
      while((strtemp=in->readLine().trimmed())!="</>")
      {
          str=str.append(strtemp);
      }
    //    str=str+"</font>";
        return str;
}
///////////////////////////////////////////////////////////////////////////
QStringList LocalDict::loadDict(int flag)
{
    switch(flag)
    {
    case 2:inFile=new QFile ("ClanguageA.txt");
        break;
    case 3:inFile=new QFile ("EnglishSimilarA.txt");
        break;
    case 4:inFile=new QFile ("HuaWeiA.txt");
        break;
    case 5:inFile=new QFile ("ProverbA.txt");
        break;
    case 6:inFile=new QFile ("PoemsA.txt");
        break;
    case 7:inFile=new QFile ("EngineerA.txt");
        break;
    case 8:inFile=new QFile ("ChineseA.txt");
        break;
        default:inFile=new QFile ("EngineerA.txt");
        break;
    }

    in=new QTextStream (inFile);


    if(!inFile->open(QIODevice::ReadOnly|QIODevice::Text))
    {
 //       qDebug()<<"cannot read!";

    }

  //  得到chineseA.txt

//     QFile *outFile= new QFile("EngineerA.txt");
//     outFile->open(QFile::WriteOnly|QIODevice::Text);
//     QTextStream out(outFile);
//     while(!inFile->atEnd())
//     {

//         QByteArray line;
//         line= inFile->readLine().trimmed();
//         line=line.left(line.indexOf("<"));
//         QTextCodec* gbk_codec = QTextCodec::codecForName("UTF-8");
//         QString gbk_string = gbk_codec->toUnicode(line);
//         if (!line.isEmpty())
//             out << gbk_string.trimmed()<<endl;
//     }


  // 得到chineseB.txt

//    QFile *outFile= new QFile("EngineerB.txt");
//    outFile->open(QFile::WriteOnly);
//    QTextStream out(outFile);
//    while(!inFile->atEnd())
//    {

//        QByteArray line;
//        line= inFile->readLine().trimmed();
//        line=line.right(line.length()-line.indexOf("<"));
//        QTextCodec* gbk_codec = QTextCodec::codecForName("UTF-8");
//        out.setCodec(QTextCodec::codecForName("utf-8"));
//        QString gbk_string = gbk_codec->toUnicode(line.data());
//        if (!line.isEmpty())
//            out << gbk_string.trimmed()<<endl;
//    }

    //将ChineseA的内容赋给slist

    QStringList slist;
     QByteArray line;
    while(!inFile->atEnd())
    {

       line=inFile->readLine().trimmed();
        QTextCodec* gbk_codec = QTextCodec::codecForName("GBK");
        QString gbk_string = gbk_codec->toUnicode(line);
        slist<<gbk_string.trimmed();
    }
    inFile->close();
 //   return wordList;
    return slist;
}

QString LocalDict::searchWord(QString inputWord,QStringList slist,int flag)
{

    QString str;

    int index=0;
    index=slist.indexOf(inputWord);
    if(index==-1)
        return "";
 //   qDebug()<<"index="<<index;



    switch(flag)
    {
    case 2:inFile=new QFile ("ClanguageB.txt");
        break;
    case 3:inFile=new QFile ("EnglishSimilarB.txt");
        break;
    case 4:inFile=new QFile ("HuaWeiB.txt");
        break;
    case 5:inFile=new QFile ("ProverbB.txt");
        break;
    case 6:inFile=new QFile ("PoemsB.txt");
        break;
    case 7:inFile=new QFile ("EngineerB.txt");
        break;
    case 8:inFile=new QFile ("ChineseB.txt");
        break;
        default:inFile=new QFile ("EngineerB.txt");
        break;
    }

    in=new QTextStream (inFile);
    if(!inFile->open(QIODevice::ReadOnly))
    {
 //       qDebug()<<"cannot read!";

    }
    QByteArray line;
    for(int i=0;i<index;i++)
        inFile->readLine();

     line=inFile->readLine().trimmed();
     QTextCodec* gbk_codec = QTextCodec::codecForName("utf-8");
     QString gbk_string = gbk_codec->toUnicode(line);
        str=gbk_string;

   //     qDebug()<<"str="<<str;
   //    str="<br>#include < stdio.h ><br>#include < stdlib.h ><br>int main(void)";
    return str;
}











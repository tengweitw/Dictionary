#include "network.h"
#include<QList>
#include<QDesktopServices>
netWork::netWork(QObject *parent) :
    QObject(parent)
{
        http=new QNetworkAccessManager(this);
        http->setCookieJar(new QNetworkCookieJar(this));
        connect(http,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyfinished(QNetworkReply*)));
        isBusy=true;
        gotword=new Word();

}

QString netWork::getCookie(const QString &name)
{
    foreach(QNetworkCookie cookie , http->cookieJar()->cookiesForUrl(QUrl("http://www.shanbay.com/")))
    {
            //qDebug()<<cookie.name();
            if(cookie.name()==name)
            {
                return cookie.value();
            }
        }
        return "";
}


void netWork::loginShanbay()
{
       QNetworkRequest request;
       request.setUrl(QUrl("http://www.shanbay.com/accounts/login/"));
       request.setRawHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
       request.setRawHeader("Accept-Charset","GBK,utf-8;q=0.7,*;q=0.3");
       request.setRawHeader("Accept-Language","zh-CN,zh;q=0.8");
       request.setRawHeader("Cache-Control","max-age=0");
       request.setRawHeader("Connection","keep-alive");
       request.setRawHeader("Host","www.shanbay.com");
       request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 6.1) AppleWebKit/535.7 (KHTML, like Gecko) Chrome/16.0.912.63 Safari/535.7");
       request.setRawHeader("Origin","http//www.shanbay.com");
       request.setRawHeader("Referer","http://www.shanbay.com/accounts/login/");
       request.setRawHeader("Host","www.shanbay.com");
       request.setRawHeader("Content-Type","application/x-www-form-urlencoded");
       QByteArray postData;
       postData.append(QString("csrfmiddlewaretoken=%1&").arg(sessionid));
       postData.append(QString("username=%1&password=%2&").arg(QUrl::toPercentEncoding(username).constData()).arg(password));
       postData.append("login=登录&continue=home&u=1&next=");
       request.setHeader(QNetworkRequest::ContentLengthHeader,postData.size());
       httpAction=LoginAction;
       http->post(request,postData);
}

void netWork::queryWord(const QString &word)
{
      QNetworkRequest request;
      request.setUrl(QUrl("http://www.shanbay.com/api/word/"+word));
      request.setRawHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
      request.setRawHeader("Accept-Charset","GBK,utf-8;q=0.7,*;q=0.3");
      request.setRawHeader("Accept-Language","zh-CN,zh;q=0.8");
      request.setRawHeader("Cache-Control","max-age=0");
      request.setRawHeader("Connection","keep-alive");
      request.setRawHeader("Host","www.shanbay.com");
      request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 6.1) AppleWebKit/535.7 (KHTML, like Gecko) Chrome/16.0.912.63 Safari/535.7");
      httpAction=QueryWordAction;
      http->get(request);
}

void netWork::queryExamples(QString learningid)
{
    if(learningid=="0"){
           return;
       }
       QNetworkRequest request;
       request.setUrl(QUrl("http://www.shanbay.com/api/learning/examples/"+learningid));
       request.setRawHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
       request.setRawHeader("Accept-Charset","GBK,utf-8;q=0.7,*;q=0.3");
       request.setRawHeader("Accept-Language","zh-CN,zh;q=0.8");
       request.setRawHeader("Cache-Control","max-age=0");
       request.setRawHeader("Connection","keep-alive");
       request.setRawHeader("Host","www.shanbay.com");
       request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 6.1) AppleWebKit/535.7 (KHTML, like Gecko) Chrome/16.0.912.63 Safari/535.7");
       httpAction=QueryWordExamplesAction;
       http->get(request);
}


void netWork::addWord(const QString &word)
{
    if(word.isEmpty())
    {
        qDebug()<<"你添加的内容有错！";

    }
    else
    {
        QNetworkRequest request;
        request.setUrl(QUrl("http://www.shanbay.com/api/learning/add/"+word));
        request.setRawHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
        request.setRawHeader("Accept-Charset","GBK,utf-8;q=0.7,*;q=0.3");
        request.setRawHeader("Accept-Language","zh-CN,zh;q=0.8");
        request.setRawHeader("Cache-Control","max-age=0");
        request.setRawHeader("Connection","keep-alive");
        request.setRawHeader("Host","www.shanbay.com");
        request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 6.1) AppleWebKit/535.7 (KHTML, like Gecko) Chrome/16.0.912.63 Safari/535.7");
        httpAction=AddWordAction;
        http->get(request);
    }
}

void netWork::connectNet(QString username, QString password)
{
        this->username=username;
        this->password=password;
         QNetworkRequest request;
        request.setUrl(QUrl("http://www.shanbay.com/accounts/login/"));
        request.setRawHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
        request.setRawHeader("Accept-Charset","GBK,utf-8;q=0.7,*;q=0.3");
        request.setRawHeader("Accept-Language","zh-CN,zh;q=0.8");
        request.setRawHeader("Cache-Control","max-age=0");
        request.setRawHeader("Connection","keep-alive");
        request.setRawHeader("Host","www.shanbay.com");
        request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 6.1) AppleWebKit/535.7 (KHTML, like Gecko) Chrome/16.0.912.63 Safari/535.7");
        httpAction=GetSessionidAction;
        http->get(request);
}

void netWork::replyfinished(QNetworkReply *reply)
{
    QVariant status_code=reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
 //  qDebug()<<"code_state="<<status_code;
    if(status_code==QVariant::Invalid)
        emit NetState(false);
    else
        emit NetState(true);
    QJsonDocument jsonDoc;
    QJsonObject jsonObj,vocObj,en_definitionsObj;

    switch(httpAction)
    {
    case NoAction:
        break;
    case GetSessionidAction:
        sessionid=getCookie("csrftoken");
        if(!sessionid.isEmpty()){
            emit connectSuccess();

       //     qDebug()<<("已经连接扇贝网，正在验证用户名密码...");
           loginShanbay();
        }else{
            emit connectFail();

        //     qDebug()<<("无法连接扇贝网,请检查网络");
        }

        break;
    case LoginAction:
        httpAction=NoAction;
        if(0==reply->readAll().size())
        {
            QString nickname=QUrl::fromPercentEncoding(getCookie("username").toLatin1());
            emit verifySuccess();

      //      qDebug()<<"用户已经登录"<<nickname;
        }else
        {
            emit verifyFail();
      //      qDebug()<<"登录失败！用户名或者密码错误";
        }
        break;
    case QueryWordAction:
      //  qDebug()<<"query word----";

        jsonDoc=QJsonDocument::fromJson(reply->readAll());
        if(!jsonDoc.isNull())
        {
            jsonObj=jsonDoc.object();
            vocObj=jsonObj.value("voc").toObject();
            en_definitionsObj=vocObj.value("en_definitions").toObject();   
            gotword->clear();

            gotword->name=vocObj.value("content").toString();//单词本身

            gotword->learning_id=QString::number(jsonObj.value("learning_id").toDouble(),'g',15);
            gotword->definition=vocObj.value("definition").toString();//中文解释
            gotword->pron=vocObj.value("pron").toString();//单词音标


            gotword->pron=gotword->pron;
            gotword->audio=vocObj.value("audio").toString();

            for( QJsonObject::const_iterator it=en_definitionsObj.constBegin();it!=en_definitionsObj.constEnd();it++)
            {
                gotword->en_definitions+=QString("<br><font color='black'><b>%1. </b></font>").arg(it.key());
                if(it.value().isArray())
                {
                    foreach(QJsonValue jv,it.value().toArray())
                    {
                       gotword->en_definitions+=QString("%1; ").arg(jv.toString());
                    }
                }
                gotword->en_definitions+="<br>";

            }

            emit signalQueryFinished(gotword);

//            qDebug()<<"name="<<gotword->name;
//            qDebug()<<"word="<<gotword->learning_id;
//            qDebug()<<"definition="<<gotword->definition;
//            qDebug()<<"en_definitions="<<gotword->en_definitions;
//            qDebug()<<"pron="<<gotword->pron;
//            qDebug()<<"audio"<<gotword->audio;
            if(gotword->learning_id!="0")//不是新单词
            {

             //   qDebug()<<"你学过这个单词";
            }

        }

        break;

    case QueryWordExamplesAction:
//            qDebug()<<"query word examples";
//             qDebug()<<reply->readAll();

            break;

    case AddWordAction:
//        qDebug()<<"add word";
        jsonDoc=QJsonDocument::fromJson(reply->readAll());
        if(!jsonDoc.isNull())
        {
            jsonObj=jsonDoc.object();
            //qDebug()<<"jsonObj"<<jsonObj;
            gotword->learning_id=QString::number(jsonObj.value("id").toDouble(),'g',15);
          //  qDebug()<<jsonObj.value("id").toDouble()<<"add word result learning id="<<gotword->learning_id<< (gotword->learning_id!="0");
         //   emit signalAddwordFinished(DICT::word->learning_id!="0");
         //   queryExamples(DICT::word->learning_id);
        }



    default:break;
    }
}





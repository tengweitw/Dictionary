#include "word.h"

Word::Word()
{

}
Word::Word(const Word &w)
{
    name=w.name;
    learning_id=w.learning_id;
    definition=w.definition;
    en_definitions=w.en_definitions;
    pron=w.pron;
    audio=w.audio;
    examples=w.examples;
    player=new QMediaPlayer();

}

Word::~Word()
{
}


void Word::play(QString mp3)
{
 //   qDebug()<<mp3;
   // mp3=mp3.replace("sounds","audio/us");
  //  qDebug()<<"okok";

     player->setMedia(QUrl("http://media.shanbay.com/audio/us/hello.mp3"));
//    qDebug()<<"okok1";
    player->play();
 //   qDebug()<<mp3;

}

void Word::clear()
{
    name="";
    learning_id="";
    definition="";
    en_definitions="";
    pron="";
    audio="";
   // this->examples=NULL;
}

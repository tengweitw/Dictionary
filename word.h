#ifndef WORD_H
#define WORD_H
#include<QString>
#include<List>
#include <QJsonObject>
#include<QtMultimedia/QMediaPlayer>
#include<QUrl>
class Word
{
public:
    explicit Word();
    Word(const Word& word);
    ~Word();
public:

    QString name;
    QString learning_id;
    QString definition;
    QString en_definitions;
    QString pron;
    QString audio;
    QMediaPlayer *player;
   QList<QJsonObject> *examples;

    void play(QString mp3);
    void clear();
};

#endif // WORD_H

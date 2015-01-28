#ifndef LOCALDICT_H
#define LOCALDICT_H

#include<QTextStream>
#include<QFile>
#include<QFont>
#include<QString>
#include<QStringList>
#include<QList>
#include<QDebug>
#include<QByteArray>
#include<QStringList>
class LocalDict
{
public:

    QFile *inFile;
    QFile *outFile;
    QTextStream *in;

    QString strPron;
    QStringList wordList;
    LocalDict();

    QStringList loadDictWeiKe();
    QString searchWordWeiKe(QString inputWord);

    QStringList loadDict(int flag);
    QString searchWord(QString inputWord,QStringList slist,int flag);
};

#endif // LOCALDICT_H

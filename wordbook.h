#ifndef WORDBOOK_H
#define WORDBOOK_H

#include"basicwindow.h"
#include"wordmodify.h"
#include <QWidget>
#include<QTableWidgetItem>
#include<QDebug>

namespace Ui {
class WordBook;
}

class WordBook : public BasicWindow
{
    Q_OBJECT
    QString txt;
public:
    explicit WordBook(BasicWindow *parent = 0);
    ~WordBook();
    int row;//记录当前行
private:
    Ui::WordBook *ui;

private slots:

    void on_tableWidget_doubleClicked(const QModelIndex &index);
    void on_wordMinBtn_clicked();
    void on_wordCloseBtn_clicked();
    void on_wordReviewBtn_clicked();
    void on_wordScanBtn_clicked();

    void replyShowWord(QString);
    void on_wordAddBtn_clicked();
    void on_wordEditBtn_clicked();

    void on_wordOmitBtn_clicked();

    void on_tableWidget_clicked(const QModelIndex &index);

    void on_wordSynBtn_clicked();

signals:
    void addWord(QString);
    void editWord(QString,int);
    void deleteWord(QString,int);

};

#endif // WORDBOOK_H

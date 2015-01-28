#ifndef SPEECH_H
#define SPEECH_H

#include <QObject>


class QtSpeech : public QObject {
    Q_OBJECT
public:
    // 处理异常情况
    struct Error { QString msg; Error(QString s):msg(s) {} };
    struct InitError : Error { InitError(QString s):Error(s) {} };
    struct LogicError : Error { LogicError(QString s):Error(s) {} };
    struct CloseError : Error { CloseError(QString s):Error(s) {} };

    //定义数据类型
    struct VoiceName { QString id; QString name; };
    typedef QList<VoiceName> VoiceNames;

    //定义构造函数
    QtSpeech(QObject * parent);
    QtSpeech(VoiceName n = VoiceName(), QObject * parent =0L);
    virtual ~QtSpeech();

    const VoiceName & name() const; //要读的内容
    static VoiceNames voices();     //要读的内容

    void say(QString) const;                                    //同步发音
    void tell(QString) const;                                   //异步发音
    void tell(QString, QObject * obj, const char * slot) const; //发音结束时，有停顿
    /*******************/
    void pause(void) const;//暂停
    void resume(void) const;//从暂停中恢复
    void stop(void) const;//停止发音

    /******************/

signals:
    void finished();

protected:
    virtual void timerEvent(QTimerEvent *);

private:
    class Private;
    Private * d;
};

//}

#endif // SPEECH_H

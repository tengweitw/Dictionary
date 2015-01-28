#ifndef SPEECH_H
#define SPEECH_H

#include <QObject>


class QtSpeech : public QObject {
    Q_OBJECT
public:
    // �����쳣���
    struct Error { QString msg; Error(QString s):msg(s) {} };
    struct InitError : Error { InitError(QString s):Error(s) {} };
    struct LogicError : Error { LogicError(QString s):Error(s) {} };
    struct CloseError : Error { CloseError(QString s):Error(s) {} };

    //������������
    struct VoiceName { QString id; QString name; };
    typedef QList<VoiceName> VoiceNames;

    //���幹�캯��
    QtSpeech(QObject * parent);
    QtSpeech(VoiceName n = VoiceName(), QObject * parent =0L);
    virtual ~QtSpeech();

    const VoiceName & name() const; //Ҫ��������
    static VoiceNames voices();     //Ҫ��������

    void say(QString) const;                                    //ͬ������
    void tell(QString) const;                                   //�첽����
    void tell(QString, QObject * obj, const char * slot) const; //��������ʱ����ͣ��
    /*******************/
    void pause(void) const;//��ͣ
    void resume(void) const;//����ͣ�лָ�
    void stop(void) const;//ֹͣ����

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

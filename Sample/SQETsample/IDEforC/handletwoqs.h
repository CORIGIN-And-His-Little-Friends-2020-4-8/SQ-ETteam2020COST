#ifndef HANDLETWOQS_H
#define HANDLETWOQS_H
#include<Qsci/qsciscintilla.h>
#include <QObject>
#include <QString>
#include <QDebug>
#include <QChar>
class HandleTwoQs : public QObject
{
    Q_OBJECT
public:
    explicit HandleTwoQs(QsciScintilla *,QsciScintilla *,QObject *parent = nullptr);

private:
    QsciScintilla *visibleEdit;         //用户直接看见的文本框
    QsciScintilla *inVisibleEdit;       //用户不可见的保存注释的文本框
    int H1;             //光标初始行
    int L1;             //光标初始列
    int H2;             //光标结束行
    int L2;             //光标结束列
    bool selectexist;   //是否有未处理选区
    bool textChange;    //文本是否被改变
    bool hasEndofLine1;  //选区起始是一行的末尾
    bool hasEndofLine2; //选区结束也是一行的末尾
    bool hasEndofLine;  //选区后有删除行的行为
    bool needHandle;    //备用
    int posOrder();                             //判断光标始末位置的前后次序
    void orserPor(int*,int*,int*,int*);         //给光标位置排个序
    QString deletecomment();
    void lauchcopy();
    bool kuohao(QString *);             //括号补全
signals:

public slots:
    void hanCursorChange(int,int);              //处理光标移动事件
    void hanTextChange();                       //处理文本改变事件
    void removeComment();           //隐藏注释
    void reShowComment();           //显示注释

};

#endif // HANDLETWOQS_H

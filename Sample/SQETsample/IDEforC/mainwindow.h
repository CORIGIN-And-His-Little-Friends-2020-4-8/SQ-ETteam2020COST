#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qsciapis.h>
#include <QVBoxLayout>
#include <QDebug>
#include <QFont>
#include <QString>
#include <QPalette>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QFontComboBox>
#include <QPushButton>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexerjava.h>
#include <string.h>
#include <QByteArray>
#include <QComboBox>
#include <Qsci/qsciapis.h>
#include <QPoint>
#include <malloc.h>
#include "mylexerc.h"
#include "gcc_compiler_fun.h"
#include "searchdialog.h"
#include "handletwoqs.h"
//存储链表的结点
typedef struct comNode{
    int type;
    int starthpos;
    int startlpos;
    QString content;
    struct comNode *next;
}comnode,*comNODE;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    SearchDialog *sdlg;                 //搜索框
    QString myfile;                     //搜索的字符串

private:
    Ui::MainWindow *ui;
    int kansheikuai;                //看光标位置与文本变动谁快
    QTextEdit *demo;                //试一个文本框样本
    QFontComboBox *fontbox;
    QComboBox *backGroColor;        //设置背景颜色
    QsciScintilla *editor;
    QsciScintilla *temeditor;
    QString filename;               //当前文件路径
    QPushButton *changeComment;     //隐藏显现注释按钮
    HandleTwoQs *han;
    bool isShow;                    //是否隐藏注释   1为未隐藏
    bool isSaved;
    bool isExist;               //判断当前文件是否被创建
    bool isCompiled;            //判断是否已被编译
    QString convert(QString filepath);      //转换文件路径格式
    GCC_compiler_fun *com;
    MyLexerC *CLexer;
    void initQsci();                //初始化文本框
    void newcopy();
private slots:
    void dealComment();             //准备处理注释
    void on_save();//保存文件槽函数
    void on_action_O_triggered();
    void on_action_N_triggered();

    void on_action_C_2_triggered();

    void on_action_R_triggered();

    void on_action_6_triggered();
    void changedFont(const QFont& f);
    void changeStyle();     //改变背景颜色

    void on_action_F_triggered();
    void show_findText(QString str);                //搜索槽函数
    void textdiffer();                  //文本改变
signals:
    void requSave();
    void showComment();             //显示注释
    void hideComment();             //隐藏注释

};

#endif // MAINWINDOW_H

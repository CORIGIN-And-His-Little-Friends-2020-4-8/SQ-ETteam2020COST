#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gcc_compiler_fun.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fontbox = new QFontComboBox(this);
    fontbox->setFontFilters(QFontComboBox::AllFonts);
    connect(fontbox,SIGNAL(currentFontChanged(QFont)),this,SLOT(changedFont(QFont)));
    ui->mainToolBar->addWidget(fontbox);
    changeComment = new QPushButton(this);
    isShow = true;
    changeComment->setText("隐藏显示注释");
    connect(changeComment,SIGNAL(clicked(bool)),this,SLOT(dealComment()));
    ui->mainToolBar->addWidget(changeComment);
//    backGroColor = new QComboBox(this);
//    backGroColor->addItem(QWidget::tr("背景颜色1"));
//    backGroColor->addItem(QWidget::tr("背景颜色2"));
//    connect(backGroColor,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeStyle()));
    //ui->mainToolBar->addWidget(backGroColor);
    initQsci();                                         //初始化文本编辑框及词法分析器


    ui->verticalLayout->addWidget(editor);//将文本编辑框加入到布局中
    ui->verticalLayout->addWidget(temeditor);
    han = new HandleTwoQs(editor,temeditor,this);
    connect(this,SIGNAL(hideComment()),han,SLOT(removeComment()));
    connect(this,SIGNAL(showComment()),han,SLOT(reShowComment()));
    isExist = false;                                //打开窗体文本框内设为未被创建
    isCompiled = false;                             //标记当前文本未被编译
    connect(ui->action_S,SIGNAL(triggered()),this,SLOT(on_save()));//保存文件
    connect(this,SIGNAL(requSave()),this,SLOT(on_save()));         //新建文件时请求保存当前文件
    //connect(ui->action_C,&QAction::triggered,this,[=](){editor->copy();});  //复制
    connect(ui->action_C,&QAction::triggered,editor,&QsciScintilla::copy);  //复制
    connect(ui->action_P,&QAction::triggered,this,[=](){editor->paste();}); //粘贴
    connect(ui->action_T,&QAction::triggered,this,[=](){editor->cut();});    //剪切
    connect(ui->action_L,&QAction::triggered,this,[=](){editor->selectAll();});//全选
    connect(ui->action_3,&QAction::triggered,this,[=](){editor->redo();});     //反撤销
    connect(ui->action_4,&QAction::triggered,this,[=](){editor->undo();});      //撤销
    connect(ui->action_D,&QAction::triggered,this,[=](){editor->removeSelectedText();});      //删除

    connect(editor,SIGNAL(cursorPositionChanged(int,int)),this,SLOT(cursormove(int,int)));
    connect(editor,SIGNAL(textChanged()),this,SLOT(textdiffer()));

}
//处理文本改变槽函数
void MainWindow::textdiffer()
{
    kansheikuai = 1;
    //qDebug()<<"textchange"<<kansheikuai;
    if(editor->lineLength(0)>5)
    {
        //editor->setSelection(0,0,0,5);
        //qDebug()<<editor->selectedText();
    }
    int h,l;
    editor->getCursorPosition(&h,&l);
    //qDebug()<<h<<l;
}

//初始化界面
void MainWindow::initQsci()
{
    editor = new QsciScintilla(this);
    temeditor = new QsciScintilla(this);
//    QsciLexerCPP *textLexer = new QsciLexerCPP;//创建一个词法分析器
    //editor->setLexer(textLexer);//给QsciScintilla设置词法分析器
    //QsciLexerJava *textLexer = new QsciLexerJava;//创建一个词法分析器
    CLexer = new MyLexerC;//创建一个词法分析器
    editor->setLexer(CLexer);//给QsciScintilla设置词法分析器
    CLexer->setPaper(Qt::white);
    CLexer->setFoldAtElse(true);          //}else{行可折叠
    CLexer->setFoldComments(true);
    sdlg=new SearchDialog(this);
    QObject::connect(sdlg, SIGNAL(sendString(QString)), this, SLOT(show_findText(QString)));

    editor->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,QsciScintilla::SC_CP_UTF8);//设置为UTF-8编码

    editor->setMarginType(0,QsciScintilla::NumberMargin);//设置编号为0的页边显示行号

    editor->setMarginLineNumbers(0,true);//对该页边启用行号

    editor->setMarginWidth(0,50);//设置页边宽度

    editor->setAutoCompletionCaseSensitivity(true);//设置大小写敏感

    editor->setAutoCompletionSource(QsciScintilla::AcsAll);//补全之前的文本

    editor->setAutoCompletionThreshold(2);//每输入1个字符就出现自动完成的提示

    editor->setAutoIndent(true);//开启自动缩进

    editor->setIndentationGuides(QsciScintilla::SC_IV_LOOKBOTH);//设置缩进的显示方式

    editor->setCaretLineVisible(true);//显示选中的行号

    editor->setCaretLineBackgroundColor(Qt::lightGray);//显示选中行号的背景色

    editor->setMarginsBackgroundColor(QColor("#FFCCFF")); //左侧行号显示的背景色

    editor->setCaretLineVisible(true);//设置自动补全

    editor->setBraceMatching(QsciScintilla::SloppyBraceMatch);//设置括号匹配

    editor->setMatchedBraceBackgroundColor(QColor("#00FF00"));//设置括号匹配背景色

    editor->setFolding(QsciScintilla::BoxedTreeFoldStyle,2);//设置代码块可折叠
    QsciAPIs *apis = new QsciAPIs(CLexer);
    apis->add(QString("basic"));
    apis->prepare();

}

//主界面析构函数
MainWindow::~MainWindow()
{
    delete ui;
}

//保存文件
void MainWindow::on_save()
{
    if(!isShow) {emit showComment();isShow = true;}

    //QFileDialog::getSaveFileName
    if(!isExist)        //如当前文件未被创建，则新建
    {

        filename = QFileDialog::getSaveFileName(this, tr("保存"),
                                 "./c.c",
                                tr("text (*.txt *.c *.cpp)"));

        if(filename.isEmpty())
        {
            ui->statusBar->showMessage(QString("保存失败"));
            return;
        }
    }
    QFile file;
    file.setFileName(filename);//初始化

    this->setWindowTitle(filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))//如果文件已经存在或者里面有内容需要覆盖，用清除操作Truncate
    {                          //WriteOnly:如果没有文件的话它会自动创建，有新建的功能
        ui->statusBar->showMessage(QString("保存失败"));
        return ;
    }

    QTextStream out(&file);//写入文件操作用out
    // QString str=out.write();
    QByteArray tem = editor->text().toUtf8();
    out<<tem;
    editor->setModified(false);
    isExist = true;         //标记当前文件已被创建
    QMessageBox::information(this,"提示","已保存");
    file.close();
}

//打开文件
void MainWindow::on_action_O_triggered()
{
    if(editor->isModified())       //如文本框内改变
    {
        int nRet = QMessageBox::question(NULL, "当前文件未保存","是否保存", QMessageBox::Yes, QMessageBox::No);        //询问是否保存当前文档
        if (QMessageBox::Yes == nRet) {
            qDebug()<<isExist;
              emit requSave();
        }
    }
    filename=QFileDialog::getOpenFileName(this, tr("打开"),
                                          "",
                                         tr("text (*.txt *.c *.cpp)"));
    if(filename.isEmpty())  return;//若关闭了打开文件窗口，则终止打开文件操作
    QFile file;
    file.setFileName(filename);//初始化
    if(!file.open(QFile::ReadOnly | QFile::Text)){
         QMessageBox::information(this,"错误","打开文件失败");
         return;
    }
    QTextStream in(&file);
    editor->setText(in.readAll().toUtf8());
    file.close();                                   //关闭文件
    editor->setModified(false);                 //标记当前文本未改变
    isExist = true;                             //标记当前文本有保存路径
}

//新建文件
void MainWindow::on_action_N_triggered()
{
    qDebug()<<editor->isModified();
    if(!editor->isModified())       //如文本框内未改变
    {
        editor->clear();            //直接清空文本框
    }
    else
    {
        int nRet = QMessageBox::question(NULL, "当前文件未保存","是否保存", QMessageBox::Yes, QMessageBox::No);        //询问是否保存当前文档
        if (QMessageBox::Yes == nRet) {
              emit requSave();
        }
        else
        {

        }
        editor->clear();
        editor->setModified(false);
    }
}

//编译文件
void MainWindow::on_action_C_2_triggered()
{
    if(editor->isModified())                        //判断文件是否被保存
    {
        this->on_save();
    }
    QString newpath = convert(filename);
    QByteArray ba = newpath.toLocal8Bit(); // must
    char *newpath1;
    newpath1 = ba.data();
    com = new GCC_compiler_fun(newpath1);
    com->compileFile();

    if(com->judgeError()==1)
    {
        ui->textBrowser->append("compile success!");
        isCompiled=true;
    }
    else
    {
        QString errinfo = QString::fromStdString(com->getErrorInfo());
        ui->textBrowser->append(errinfo);
        isCompiled=false;
    }

}

//转换文件路径格式,对接GCC调用接口
QString MainWindow::convert(QString filepath)
{
    QStringList  tem = filepath.split("/");         //划分原有的文件路径
    QString newpath = tem.at(0);                          //初始化新文件路径
    for(int i = 1;i<tem.size();i++)
    {

        newpath = newpath + "\\";
        newpath = newpath + tem.at(i);
    }
    return newpath;
}

//运行程序
void MainWindow::on_action_R_triggered()
{
    if(!isCompiled)
    {
        QMessageBox::warning(NULL, "warning", "当前文件未编译", QMessageBox::Yes);
    }
    else
    {
        com->executeRes();
    }
}

//另存为
void MainWindow::on_action_6_triggered()
{
    if(!isShow) {emit showComment();isShow = true;}
    filename = QFileDialog::getSaveFileName(this, tr("保存"),
                             "./c.c",
                            tr("text (*.txt *.c *.cpp)"));

    if(filename.isEmpty())
    {
        ui->statusBar->showMessage(QString("保存失败"));
        return;
    }
    QFile file;
    file.setFileName(filename);//初始化

    this->setWindowTitle(filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))//如果文件已经存在或者里面有内容需要覆盖，用清除操作Truncate
    {                          //WriteOnly:如果没有文件的话它会自动创建，有新建的功能
        ui->statusBar->showMessage(QString("保存失败"));
        return ;
    }
    QTextStream out(&file);//写入文件操作用out
    QByteArray tem = editor->text().toUtf8();
    out<<tem;
    editor->setModified(false);
    isExist = true;         //标记当前文件已被创建
}

//改变字体
void MainWindow::changedFont(const QFont& f)
{
    CLexer->setFont(f);
}

//改变风格
void MainWindow::changeStyle()
{

    QFont ft1,ft2,ft3,ft4;
    ft1.setPointSize(10);
    ft2.setPointSize(20);
    ft3.setPointSize(30);
    ft4.setPointSize(40);
    QPalette pa1,pa2,pa3,pa4;
    pa1.setColor(QPalette::WindowText,Qt::red);
    pa2.setColor(QPalette::WindowText,Qt::green);
    pa3.setColor(QPalette::WindowText,Qt::yellow);
    pa4.setColor(QPalette::WindowText,Qt::blue);
    QPalette pal;
    pal.setColor(QPalette::Text, QColor(111,0,0));
    QPalette pall;
    pall.setColor(QPalette::Text, QColor(111,200,100));
    QPalette palll;
    palll.setColor(QPalette::Base, QColor(155,123,123));
    QPalette pallll;
    pallll.setColor(QPalette::Base, QColor(155,23,123));


    if(backGroColor->currentIndex()==0) editor->setBackgroundRole(QPalette::Window);
    if(backGroColor->currentIndex()==1) editor->setBackgroundRole(QPalette::Window);

}

//查找
void MainWindow::on_action_F_triggered()
{
    sdlg->show();
}

//查找下一个
void MainWindow::show_findText(QString str)
{
    QString expr = str;
    if(editor->findFirst(expr,false,true,false,false)){

    }
    else
    {
        QMessageBox::information(this,tr("注意"),tr("没有找到内容"),QMessageBox::Ok);
        editor->setCursorPosition(QTextCursor::Start, QTextCursor::MoveAnchor); //将光标置于开始位置
    }
}

//准备处理注释
void MainWindow::dealComment()
{
    qDebug()<<isShow;
    if(isShow) {emit hideComment();isShow = false;}
    else {emit showComment();isShow = true;}
}



#include "handletwoqs.h"

HandleTwoQs::HandleTwoQs(QsciScintilla *a,QsciScintilla *b,QObject *parent) : QObject(parent)
{
    this->visibleEdit = a;
    this->inVisibleEdit = b;
    connect(visibleEdit,SIGNAL(cursorPositionChanged(int,int)),this,SLOT(hanCursorChange(int,int)));
    connect(visibleEdit,SIGNAL(textChanged()),this,SLOT(hanTextChange()));
    //connect(visibleEdit,SIGNAL())
    H1 = 0;
    L1 = 0;
    H2 = 0;
    L2 = 0;
    selectexist = false;
    textChange = false;
    needHandle = false;
    hasEndofLine = false;
    hasEndofLine1 = false;
    hasEndofLine2 = false;
}

//处理光标位置改变事件
void HandleTwoQs::hanCursorChange(int ch, int cl)
{
    qDebug()<<"光标移动了";
    if(visibleEdit->hasSelectedText())              //若有选区
    {
        selectexist = true;                         //标记存在选区
        visibleEdit->getCursorPosition(&H2,&L2);    //获取选区结束点
        qDebug()<<"看看选区"<<L1<<visibleEdit->lineLength(H1)<<L2<<visibleEdit->lineLength(H2);
        if((H1<visibleEdit->lines()-1&&L1==visibleEdit->lineLength(H1)-2)||(H1==visibleEdit->lines()-1&&L1==visibleEdit->lineLength(H1)))
        {
            hasEndofLine1 = true;
            qDebug()<<"起始选到末尾了";
        }
        if((H2<visibleEdit->lines()-1&&L2==visibleEdit->lineLength(H2)-2)||(H2==visibleEdit->lines()-1&&L2==visibleEdit->lineLength(H2)))
        {
            hasEndofLine2 = true;
            qDebug()<<"终止选到末尾了";
        }

    }
    else if(textChange&&!needHandle)                             //无选区但先前文本被改变
    {
        textChange = false;
        if(posOrder()>0)//输入了一个文字
        {
            visibleEdit->setSelection(H1,L1,H2,L2);
            QString kuo = visibleEdit->selectedText();
            inVisibleEdit->insertAt(visibleEdit->selectedText(),H1,L1);
            visibleEdit->setCursorPosition(H2,L2);
            if(kuohao(&kuo))
            {
                qDebug()<<kuo;
                visibleEdit->insertAt(kuo,H2,L2);
                kuo = visibleEdit->selectedText() + kuo;
                inVisibleEdit->insertAt(kuo,H1,L1+1);
            }
        }
        else if(posOrder()==0)//删除了一个文字
        {
            inVisibleEdit->setSelection(H2,L2,H1,L1+1);

            QString t = "";
            inVisibleEdit->replaceSelectedText(t);
        }
        else if(posOrder()<0)
        visibleEdit->getCursorPosition(&H1,&L1);
        textChange = false;
    }
    else if(needHandle&&textChange)
    {
        needHandle = false;
        textChange = false;
        int ch,cl;
        visibleEdit->getCursorPosition(&ch,&cl);
        int h1,l1,h2,l2;
        orserPor(&h1,&l1,&h2,&l2);      //定位选区
        qDebug()<<"替换选区"<<h1<<l1<<h2<<l2<<ch<<cl;
        if(ch!=h1||cl!=l1)
        {
            visibleEdit->setSelection(h1,l1,ch,cl);
            inVisibleEdit->insertAt(visibleEdit->selectedText(),h1,l1);
            visibleEdit->setCursorPosition(ch,cl);
        }
        visibleEdit->getCursorPosition(&H1,&L1);
    }
    else
    {
        visibleEdit->getCursorPosition(&H1,&L1);

        textChange = false;
    }
    needHandle = false;
}

//处理文本改变事件
void HandleTwoQs::hanTextChange()
{

    if(selectexist)
    {
        selectexist = false;
        int h1,l1,h2,l2;
        orserPor(&h1,&l1,&h2,&l2);      //定位选区
        if(hasEndofLine)
        {
            if(h2 == visibleEdit->lines()-1)
            {l2=inVisibleEdit->lineLength(h2);qDebug()<<"文本末尾了";}
            else
            l2=inVisibleEdit->lineLength(h2)-2;
        }
        inVisibleEdit->setSelection(h1,l1,h2,l2);
        qDebug()<<h1<<l1<<h2<<l2;
        QString t = "";
        inVisibleEdit->replaceSelectedText(t);
        needHandle = true;
        hasEndofLine = false;
        textChange = false;
    }
    else
    {
        visibleEdit->getCursorPosition(&H1,&L1);        //保存光标初始位置
        visibleEdit->setCursorPosition(H1,L1);
        textChange = true;
    }
    qDebug()<<"文本改变信号"<<textChange<<needHandle;
}

//判断光标始末位置的前后次序 -1末大于始
int HandleTwoQs::posOrder()
{
    visibleEdit->getCursorPosition(&H2,&L2);            //获取当前光标位置
    if(H1 == H2)
    {
        if(L1 == L2)            //光标始末位置相同
            return 0;
        else return L1<L2?1:-1;
    }
    else return H1<H2?1:-1;
}

//给光标始末位置排序
void HandleTwoQs::orserPor(int *h1, int *l1, int *h2, int *l2)
{
    if(H1 == H2)
    {
        *h1=H1;
        *h2=H2;
        if(L1<=L2)
        {
            *l1=L1;
            *l2=L2;
            if(hasEndofLine2){hasEndofLine = true;hasEndofLine2 = false;}
        }
        else
        {
            *l1=L2;
            *l2=L1;
            if(hasEndofLine1){hasEndofLine = true;hasEndofLine1 = false;}
        }
    }
    else
    {
        if(H1<H2){
            *h1=H1;*h2=H2;*l1=L1;*l2=L2;
            if(hasEndofLine2){hasEndofLine = true;hasEndofLine2 = false;}
        }
        else    {
            *h1=H2;*h2=H1;*l1=L2;*l2=L1;
            if(hasEndofLine1){hasEndofLine = true;hasEndofLine1 = false;}
        }
    }
}

//隐藏注释
void HandleTwoQs::removeComment()
{

    inVisibleEdit->setText(visibleEdit->text());
    visibleEdit->setText(deletecomment());
    selectexist = false;        //重置状态
    textChange = false;
    needHandle = false;
    hasEndofLine = false;
    hasEndofLine1 = false;
    hasEndofLine2 = false;
}

//重新展示注释
void HandleTwoQs::reShowComment()
{

    visibleEdit->setText(inVisibleEdit->text());
    selectexist = false;        //重置状态
    textChange = false;
    needHandle = false;
    hasEndofLine = false;
    hasEndofLine1 = false;
    hasEndofLine2 = false;
}

//删除文本中的注释
QString HandleTwoQs::deletecomment()
{
    QString strLine;
    QString tail = "\r\n";
    QString final = "";
    int m_type = 0;
    int pos = -1;
    for(int i=0;i<visibleEdit->lines();i++)
    {
        strLine = visibleEdit->text(i);
        //查找模式
        if(m_type == 0)
        {
            int pos1 = strLine.indexOf("//");
            int pos2 = strLine.indexOf("/*");
            int pos3 = strLine.indexOf("*/");

            //找到'//'
            if(pos1 > -1)
            {
                //同时也找到了'/*'，并且'//'在前面，那么//后面的全是注释，同时模式为查找模式
                if(pos1 < pos2)
                {
                    pos = pos1;
                    m_type = 0;
                }
                else
                {
                    //同时也找到了'/*'，并且'//'在后面，那么模式为查找'*/'，中间的全部都是注释
                    if(pos2 > -1)
                    {
                        pos = pos2;
                        m_type = 1;
                    }
                    //如果没有'/*'，那这一行为注释行
                    else
                    {
                        pos = pos1;
                        m_type = 0;
                    }
                }
            }
            else
            {
                //没有找到'//',但是找到了'/*'
                if(pos2 > -1)
                {
                    pos = pos2;
                    m_type = 1;
                }
                //什么也没找到，继续寻找
                else
                {
                }
            }

            //如果同时找到了'/*'和'*/',将中间的注释去掉
            if(pos2 > -1 && pos3 > -1)
            {
                QString strMsg = strLine.mid(pos2, pos3-pos2+2);
                qDebug()<<strMsg;
                strLine = strLine.remove(strMsg);
                pos = -1;
                m_type = 0;
            }
        }
        else
        {
            //查找'*/'模式，如果找到，'*/'前面的全部移除，没有找到那么从0开始整行移除
            int pos3 = strLine.indexOf("*/");
            if(pos3 > -1)
            {
                m_type = 2;
                pos = pos3;
            }
            else
            {
                pos = 0;
            }
        }

        //POS来表明是否为需要处理的注释
        if(pos > -1)
        {
            //如果是'//','/*'都是移除后面的，'*/'移除前面的
            if(m_type != 2)
            {
                QString strMsg = strLine.mid(pos);
                qDebug()<<strMsg;
                strLine = strLine.left(pos);
                if(i!=visibleEdit->lines()-1)
                strLine = strLine+tail;
            }
            else
            {
                QString strMsg = strLine.left(pos+2);
                qDebug()<<strMsg;
                strLine = strLine.mid(pos+2);

                m_type = 0;
            }

            //除非是开始模式，否则全部置-1，处理已经完毕
            if(m_type != 1)
            {
                pos = -1;
            }
        }
        if(!strLine.isEmpty())
        {
            final = final+strLine;
        }
        else if(i!=visibleEdit->lines()-1)
        {
            final = final+tail;
        }
    }
    return final;
}

//括号补全
bool HandleTwoQs::kuohao(QString *re)
{
    if(re->size()>1)    return false;
    char*  ch;
    QByteArray ba = re->toLocal8Bit(); // must
    ch=ba.data();
    char tem=ch[0], res;
    bool revalue = false;
    QString rea;
    switch(tem)
    {
        case '(': rea=")";revalue = true;break;
        case '[': rea="]";revalue = true;break;
        case '{': rea="}";revalue = true;break;
        case '\'': rea="\'";revalue = true;break;
        case '\"': rea="\"";revalue = true;break;
    default:return false;break;
    }
    *re = rea;
    qDebug()<<*re;
    return revalue;
}

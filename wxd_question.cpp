#include "wxd_question.h"
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QTime>
#include <QCoreApplication>
#include <QTextCodec>

wxd_question::wxd_question()
{
    if(!QFile::exists(QCoreApplication::applicationDirPath() + "/WA.txt"))
    {
        QMessageBox::warning(NULL,"注意：","没有找到【错题库】。如果这是你第一次运行请忽略，否则请检查文件是否存在。");
    }
    wxd_wrong_lib = new QFile(QCoreApplication::applicationDirPath() + "/WA.txt");
    if(!wxd_wrong_lib->open(QIODevice::Append | QIODevice::Text))
    {
        QMessageBox::warning(NULL,"注意：","打开【错题库】文件失败！");
        return;
    }
//===========================================================================================================
    if(!QFile::exists(QCoreApplication::applicationDirPath() + "/A.txt"))
    {
        QMessageBox::warning(NULL,"注意：","没找到题库文件或者题库文件不存在！请拷贝对应等级题库到本程序所在目录。(" + QCoreApplication::applicationDirPath() + ")");
        return;
    }
    wxd_lib = new QFile(QCoreApplication::applicationDirPath() + "/A.txt");
    if(!wxd_lib->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(NULL,"注意：","打开题库文件失败！");
        return;
    }
}

wxd_question::wxd_question(QString lib_path, QString wlib_path)
{
    qDebug()<<lib_path << wlib_path;
    if(!QFile::exists(wlib_path))
    {
        QMessageBox::warning(NULL,"注意：","没有找到【错题库】。如果这是你第一次运行请忽略，否则请检查文件是否存在。");
    }
    wxd_wrong_lib = new QFile(wlib_path);
    if(!wxd_wrong_lib->open(QIODevice::Append | QIODevice::Text))
    {
        QMessageBox::warning(NULL,"注意：","打开【错题库】文件失败！");
        return;
    }
//===========================================================================================================
    if(!QFile::exists(lib_path))
    {
        QMessageBox::warning(NULL,"注意：","没找到题库文件或者题库文件不存在！请拷贝对应等级题库到本程序所在目录。(" + QCoreApplication::applicationDirPath() + ")");
        return;
    }
    wxd_lib = new QFile(lib_path);
    if(!wxd_lib->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(NULL,"注意：","打开题库文件失败！");
        return;
    }
}

wxd_question::~wxd_question()
{
    if(wxd_lib->isOpen()) wxd_lib->close();
    if(wxd_wrong_lib->isOpen()) wxd_wrong_lib->close();
}

int wxd_question::readQuestion()
{
    if(!wxd_lib->isOpen())
    {
        QMessageBox::warning(NULL,"警告：","题库文件没有打开，不能读取。");
        return 0;
    }
    question.clear();
    for(int ttp = 0; ttp < 7; ttp++)
    {
        question.append((wxd_lib->readLine()));
    }
    //qDebug()<< question;

    answers.clear();
    QList<int> sort;
    rightAnswer = shuffle(&sort);
    for(int i=0;i<4;++i)
    {
        answers.append(question.value(sort.value(i)).remove(0,3));
    }
    pic_name = question.value(6).remove(0,3);
    return rightAnswer;

}

int wxd_question::shuffle(QList<int> *sort)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));//设置随机数种子

    int rand;
    int ra = 3;
    *sort << 4 << 4 << 4 << 4;
    for(int j = 0; j<4; ++j)
    {
        rand = qrand() % 4 + 2;//2,3,4,5
        int i = 0;
        while ((i < j) & (sort->at(i) != rand)) i++;
        if(i == j)
        {
            sort->replace(j,rand);
            if(rand==2) ra=j;
        }
        else
        {
            if (j >0) j--;
            continue;
        }

    }
    return ra+3;
}

void wxd_question::save_wrong_lib()
{
    if(!wxd_wrong_lib->isOpen())
    {
        QMessageBox::warning(NULL,"警告：","错题库文件没有打开，不能保存。");
        return;
    }

    QTextStream out(wxd_wrong_lib);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    out.setCodec(codec);
    for(int i = 0; i < 8; i++){out << question.value(i);}
    //out << "" <<"\n";

}

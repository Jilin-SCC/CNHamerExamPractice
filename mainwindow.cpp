#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QMessageBox>
#include <QPixmap>
#include <QChar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/logo.ico"));
    ui->rbnt_s1->setText("");
    ui->rbnt_s2->setText("");
    ui->rbnt_s3->setText("");
    ui->rbnt_s4->setText("");
    timer_1s = new QTimer;
    timer_3s = new QTimer;
    timer_1m = new QTimer;
    timer_1s->start(1000);

    connect(timer_1s,&QTimer::timeout,this,&MainWindow::on_timer_1s_timeout);
    connect(timer_3s,&QTimer::timeout,this,&MainWindow::on_timer_3s_timeout);
    connect(timer_1m,&QTimer::timeout,this,&MainWindow::on_timer_1m_timeout);
    //connect(tm, &QTimer::timeout, this, [&](){this->on_timer_timeout();});

    dialog_setting = new SetDialog(this);
//    connect(dialog_setting,&QDialog::accept,this,&MainWindow::on_dialog_setting_finished);
//    connect(dialog_setting,&QDialog::rejected,this,&MainWindow::on_dialog_setting_finished);

    ExaminationTime = wxds.getValue("WXD/ExaminationTime",1024).toInt();
    counter = ExaminationTime;
    Grade = wxds.getValue( "WXD/ExaminationClass","ClassA").toString();
    finish = wxds.getValue(Grade + "/finish",1024).toInt();
    correct = wxds.getValue(Grade + "/correct",1024).toInt();
    mmax = wxds.getValue(Grade + "/max",1024).toInt();

    //qDebug()<< Grade << finish << correct << mmax ;

    QString path_lib = wxds.getValue(Grade + "/Path_lib","X.TXT").toString();
    QString path_wlib = wxds.getValue(Grade + "/Path_wlib","X_W.TXT").toString();
    qDebug()<<path_lib << path_wlib;
    QString mGrade = Grade;
    ui->label_7->setText("考试级别：" + mGrade.remove(0,5) + "  题量：" + QString::number(mmax));
    if(finish >= mmax)
    {
        ui->lab_judge_info->setText("你真厉害，题库全部刷完了一遍。");
        return;
    }
    ui->lab_judge_info->setText("开始刷题咯......");
    ui->lab_finish->setText("已完成："+ QString::number(finish) + "题");
    ui->lab_rate->setText("正确率：@_@!!!");

    wxd = new wxd_question(path_lib,path_wlib);
    for (int i = 0; i <= finish; i++) {
        right_answer = wxd->readQuestion();
    }
    display_quest(wxd);
}

MainWindow::~MainWindow()
{
    info.clear();
    if(!Grade.isEmpty())
    {
        wxds.setValue(Grade + "/finish",finish);
        wxds.setValue(Grade + "/correct",correct);
        QMessageBox::information(NULL,"保存提示：","刷题信息已经保存，下次打开时会自动载入。");
    }
    delete ui;
}


void MainWindow::on_bnt_ok_clicked()
{
    ui->bnt_ok->setEnabled(false);
    timer_1m->stop();
    if(ui->rbnt_s1->isChecked())
    {
        (judge(3))?ui->label_s1->setStyleSheet("color:green;"):ui->label_s1->setStyleSheet("color:red;");
    }
    else if(ui->rbnt_s2->isChecked() )
    {
        (judge(4))?ui->label_s2->setStyleSheet("color:green;"):ui->label_s2->setStyleSheet("color:red;");
    }
    else if(ui->rbnt_s3->isChecked())
    {
        (judge(5))?ui->label_s3->setStyleSheet("color:green;"):ui->label_s3->setStyleSheet("color:red;");
    }
    else if(ui->rbnt_s4->isChecked())
    {
        (judge(6))?ui->label_s4->setStyleSheet("color:green;"):ui->label_s4->setStyleSheet("color:red;");
    }
    else {return;}

    timer_3s->start(3000);
    counter = 3;
}

void MainWindow::on_timer_1s_timeout()
{
    ui->lcdNumber_2->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumber_2->display(--counter);
    //timer_1s->start();
}

void MainWindow::on_timer_3s_timeout()
{
    timer_3s->stop();
    counter = ExaminationTime;

    ui->lab_judge_info->setText("");
    if(finish >= mmax)
    {
        ui->lab_judge_info->setText("你真厉害，题库全部刷完了一遍。");
        return;
    }

    right_answer = wxd->readQuestion();

    display_quest(wxd);

    timer_1m->start(ExaminationTime*1000);

    ui->bnt_ok->setEnabled(true);
    timer_3s->stop();
}

void MainWindow::on_timer_1m_timeout()
{
    timer_1m->stop();
    on_bnt_ok_clicked();
}

void MainWindow::display_quest(wxd_question *wxd)
{
    ui->label_s1->setStyleSheet("color:black;");
    ui->label_s2->setStyleSheet("color:black;");
    ui->label_s3->setStyleSheet("color:black;");
    ui->label_s4->setStyleSheet("color:black;");
    //right_answer = wxd->readQuestion();
    if(right_answer == 0) {QMessageBox::warning(this,"错误：","严重错误：没能返回正确答案。");return;}
    wxd->question.value(0).remove(QChar('\n'),Qt::CaseInsensitive);
    ui->lab_num->setText(wxd->question.value(0).remove(0,3));

    wxd->question.value(1).remove(QChar('\n'),Qt::CaseInsensitive);
    ui->lab_quest->setText(wxd->question.value(1).remove(0,3));

    ui->label_s1->setText("[A]" + wxd->answers.value(0).remove(QChar('\n'),Qt::CaseInsensitive));
    ui->label_s2->setText("[B]" + wxd->answers.value(1).remove(QChar('\n'),Qt::CaseInsensitive));
    ui->label_s3->setText("[C]" + wxd->answers.value(2).remove(QChar('\n'),Qt::CaseInsensitive));
    ui->label_s4->setText("[D]" + wxd->answers.value(3).remove(QChar('\n'),Qt::CaseInsensitive));

   if (!wxd->pic_name.isEmpty() )
   {
       qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));//设置随机数种子
       int rand = qrand() % 3 + 1;
       QPixmap* ps = new QPixmap(QCoreApplication::applicationDirPath() + "/pic/"+ QString::number(rand) +".png");
       ui->label_pic->setPixmap(*ps);
   }
   else
   {
       QPixmap* ps = new QPixmap(QCoreApplication::applicationDirPath() + "/pic/" + wxd->pic_name);
       ui->label_pic->setPixmap(*ps);
   }
   //ra = wxd->rightAnswer;
}

bool MainWindow::judge(int answer)
{
    //判断是否选对
    bool r_w;
    QString result = "你的答案是：%1，正确答案是：%2，";
    if(answer == right_answer)
    {
        ui->lab_judge_info->setStyleSheet("color:green;");
        ui->lab_judge_info->setText(result.arg(QString(answer + 62)).arg(QString(62 + right_answer)) + "你答对了！");
        correct++;
        r_w = true;
    }
    else
    {
        ui->lab_judge_info->setStyleSheet("color:red;");
        ui->lab_judge_info->setText(result.arg(QString(answer + 62)).arg(QString(62 + right_answer)) + "你答错了！");
        //保存错题
        wxd->save_wrong_lib();
        r_w = false;
    }
    finish++;
    rate = (double)correct/finish*100;
    ui->lab_finish->setText("");
    ui->lab_rate->setText("");
    ui->lab_finish->setText("已完成："+ QString::number(finish) + "题");
    ui->lab_rate->setText("正确率："+ QString::number(rate,'f',2) + "%");
    return r_w;
}

void MainWindow::on_pbnt_set_clicked()
{
    if (timer_1m->isActive())
    {
        timer_1m->stop();
        sig |= 1;
        remainingTime.append(timer_1m->remainingTime());
    }
    if (timer_3s->isActive())
    {
        timer_3s->stop();
        sig |= 2;
        remainingTime.append(timer_3s->remainingTime());
    }

        timer_1s->stop();
        remainingTime.append(timer_1s->remainingTime());


    dialog_setting->setModal(true);
    dialog_setting->show();

}

//void MainWindow::on_dialog_setting_finished()
//{
//    if (sig & 1) timer_1m->start(remainingTime.value(0));
//    if (sig & 2) timer_3s->start(remainingTime.value(1));
//    timer_1s->start(remainingTime.value(2));
//}

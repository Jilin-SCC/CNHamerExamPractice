#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "setdialog.h"
#include "wxd_question.h"
#include "wxd_setting.h"
#include <QMainWindow>
#include <QList>
#include <QVariant>
#include <QDir>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int finish = 0;
    int correct = 0;
    int mmax = 0;
    double rate = 0.00f;
    QList<QVariant> info;

private slots:
    void on_bnt_ok_clicked();
    void on_timer_1s_timeout();
    void on_timer_3s_timeout();
    void on_timer_1m_timeout();

    void on_pbnt_set_clicked();
    //void on_dialog_setting_finished();

private:
    int right_answer = 1;
    Ui::MainWindow *ui;
    bool isOk;
    wxd_question *wxd;
    wxd_setting wxds;
    QTimer *timer_1s;
    QTimer *timer_3s;
    QTimer *timer_1m;
    int ExaminationTime = 0;
    int counter = 60;
    QString Grade = "A";
    QString Path_lib = QDir::currentPath();
    QString Path_wlib = QDir::currentPath();
    void display_quest(wxd_question* wxd);
    bool judge(int answer);
    char sig = 0;
    QList<int> remainingTime;
    SetDialog *dialog_setting;

};
#endif // MAINWINDOW_H

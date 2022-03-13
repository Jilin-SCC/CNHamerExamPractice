#ifndef WXD_QUESTION_H
#define WXD_QUESTION_H
#include <QFile>
#include<QString>
#include <QList>
class wxd_question
{
public:
    wxd_question();
    wxd_question(QString lib_path,QString wlib_path);
    ~wxd_question();
    QString num;
    QList<QString> question;
    QList<QString> answers;
    QString pic_name;
    int rightAnswer;

    int readQuestion();
    void save_wrong_lib();

private:
    QFile *wxd_lib;
    QFile *wxd_wrong_lib;
    int shuffle(QList<int> *sort);

};

#endif // WXD_QUESTION_H

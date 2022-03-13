#include "setdialog.h"
#include "ui_setdialog.h"
#include <QFile>
#include <QMessageBox>

SetDialog::SetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("设置");
    if(!QFile::exists(QCoreApplication::applicationDirPath() + "/libs/ClassA.txt") |
       !QFile::exists(QCoreApplication::applicationDirPath() + "/libs/ClassB.txt") |
       !QFile::exists(QCoreApplication::applicationDirPath() + "/libs/ClassC.txt") )
    {
        QMessageBox::warning(NULL,"注意：","没找到题库文件或者题库文件不存在！请拷贝对应等级题库到本程序所在目录。(" + QCoreApplication::applicationDirPath() + "/libs)");
        return;
    }
}

SetDialog::~SetDialog()
{

    delete ui;
}

void SetDialog::on_buttonBox_accepted()
{
    QString Class;
    if(ui->radioButton->isChecked())
    {
        Class = "ClassA" ;
    }
    else if(ui->radioButton_2->isChecked())
    {
        Class = "ClassB";
    }
    else if(ui->radioButton_3->isChecked())
    {
        Class = "ClassC";
    }
    else
    {
        QMessageBox::warning(NULL,"注意：","设置保存失败!");
        return;
    }

    wxds.setValue("WXD/ExaminationClass",Class);
    wxds.setValue("WXD/ExaminationTime",ui->lineEdit->text().toInt());
    wxds.setValue(Class + "/Path_lib",QCoreApplication::applicationDirPath() + "/libs/"+ Class +".txt");
    wxds.setValue(Class + "/Path_wlib",QCoreApplication::applicationDirPath() + "/libs/"+ Class +"_W.txt");
    QMessageBox::warning(NULL,"注意：","设置保存成功。\nA类考试题库：" +
                         QCoreApplication::applicationDirPath() + "/libs/ClassA.txt\nB类考试题库：" +
                         QCoreApplication::applicationDirPath() + "/libs/ClassB.txt\nC类考试题库：" +
                         QCoreApplication::applicationDirPath() + "/libs/ClassC.txt");
}

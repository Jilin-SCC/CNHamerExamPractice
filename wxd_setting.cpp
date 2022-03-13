#include "wxd_setting.h"
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QCoreApplication>

wxd_setting::wxd_setting()
{
    if(!QFile::exists(QCoreApplication::applicationDirPath() + "/s.ini"))
    {
        QMessageBox::warning(NULL,"注意：","没有找到配置文件。否则请检查文件（"+QCoreApplication::applicationDirPath()+"/s.ini）是否存在。");
        return;
    }
    wxd_set = new QSettings(QCoreApplication::applicationDirPath()+"/s.ini",QSettings::IniFormat);
}

wxd_setting::~wxd_setting()
{
    //QMessageBox::information(NULL,"保存提示：","刷题信息已经保存，下次打开时会自动载入。");
}

QList<QVariant> wxd_setting::wxd_set_load()
{
    QList<QVariant> temp;
    wxd_set->beginGroup("WXD");
    temp << wxd_set->value("Grade").toString()
         << wxd_set->value("Path_lib").toString()
         << wxd_set->value("Path_wlib").toString()
         << wxd_set->value("finish").toInt()
         << wxd_set->value("correct").toInt();
    wxd_set->endGroup();
    return temp;
}

void wxd_setting::wxd_set_save(QList<QVariant> *info)
{
    wxd_set->beginGroup("WXD");
    wxd_set->setValue("ExaminationTime",info->value(0));
    wxd_set->setValue("ExaminationClass",info->value(1));
    wxd_set->endGroup();
    wxd_set->beginGroup("ClassA");
    wxd_set->setValue("Path_lib",info->value(2));
    wxd_set->setValue("Path_wlib",info->value(3));
    wxd_set->setValue("finish",info->value(4));
    wxd_set->setValue("correct",info->value(5));
    wxd_set->endGroup();
    wxd_set->beginGroup("ClassB");
    wxd_set->setValue("Path_lib",info->value(6));
    wxd_set->setValue("Path_wlib",info->value(7));
    wxd_set->setValue("finish",info->value(8));
    wxd_set->setValue("correct",info->value(9));
    wxd_set->endGroup();
    wxd_set->beginGroup("ClassC");
    wxd_set->setValue("Path_lib",info->value(10));
    wxd_set->setValue("Path_wlib",info->value(11));
    wxd_set->setValue("finish",info->value(12));
    wxd_set->setValue("correct",info->value(13));
    wxd_set->endGroup();
}

void wxd_setting::setValue(const QString &key, const QVariant &value)
{
    wxd_set->setValue(key,value);
}

QVariant wxd_setting::getValue(const QString &key, const QVariant &defaultValue)
{
    return wxd_set->value(key,defaultValue);
}

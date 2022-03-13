#ifndef WXD_SETTING_H
#define WXD_SETTING_H
#include <QSettings>

class wxd_setting
{
public:
    wxd_setting();
    ~wxd_setting();
    QSettings *wxd_set;
    //列表元素个数 14
    QList<QVariant> wxd_set_load();
    void wxd_set_save(QList<QVariant> *info);

    void setValue(const QString &key, const QVariant &value);
    QVariant getValue(const QString &key,const QVariant &defaultValue = QVariant());
};

#endif // WXD_SETTING_H

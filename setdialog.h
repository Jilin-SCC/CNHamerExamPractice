#ifndef SETDIALOG_H
#define SETDIALOG_H
#include "wxd_setting.h"
#include <QDialog>

namespace Ui {
class SetDialog;
}

class SetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetDialog(QWidget *parent = nullptr);
    ~SetDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SetDialog *ui;
        wxd_setting wxds;
};

#endif // SETDIALOG_H

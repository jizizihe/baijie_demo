#ifndef IPSET_H
#define IPSET_H

#include <QMainWindow>
#include "ipset_interface.h"
#include "common.h"

namespace Ui {
class ipset;
}

class ipset : public QMainWindow
{
    Q_OBJECT

public:
    explicit ipset(QWidget *parent = 0);
    ~ipset();

    void language_reload();
    void ipset_font();
    void showEvent(QShowEvent *event);
    void ip_settext();

signals:
    void ret_signal();

private slots:
   // void btnChangeFlag(bool flag);

    void on_retBtn_clicked();

    void on_ipShowBtn_clicked();

    void on_setStaticIpBtn_clicked();

    void on_modStaticIpBtn_clicked();

    void on_delStaticIpBtn_clicked();

    void on_okBtn_clicked();

    void on_btn_open_clicked();

private:
    Ui::ipset *ui;
};

#endif // IPSET_H

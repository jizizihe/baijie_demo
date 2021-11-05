#ifndef IPSET_H
#define IPSET_H

#include <QWidget>
#include <QString>
#include <QProcess>
#include <QDebug>
#include <unistd.h>
#include "ipset_popup.h"
#include <QTimer>
#include <QScrollBar>

namespace Ui {
class ipset;
}

class ipset : public QWidget
{
    Q_OBJECT

public:
    explicit ipset(QWidget *parent = 0);
    ~ipset();

    ipset_popup popup;
    void language_reload();

public slots:

private slots:
    void on_retbtn_clicked();
    void on_delstaticip_clicked();
    void on_setstaticip_clicked();
    void on_modstaticip_clicked();
    void on_networkupbtn_clicked();
    void on_ipshowbtn_clicked();
    void increaseip(QString,QString);
    void modifyip(QString);
    void gobackmenu();

signals:
    void Mysignal();

private:
    Ui::ipset *ui;
};

#endif // IPSET_H

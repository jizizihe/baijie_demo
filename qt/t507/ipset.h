#ifndef IPSET_H
#define IPSET_H

#include <QWidget>
#include <QString>
#include <QProcess>
#include <QDebug>
#include <unistd.h>
#include "ipset_popup.h"
#include <QTimer>

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
    bool eventFilter(QObject *,QEvent *);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void speedtest();
    void on_pushButton_9_clicked();
    void setdynamicip(QString);
    void increaseip(QString,QString,QString);
    void modifyip(QString,QString);
    void deleteip(QString);
    void gobackmenu();

signals:
    void Mysignal();
    void StartTestSpeed();
private:
    Ui::ipset *ui;
    QTimer *timer;
};

#endif // IPSET_H

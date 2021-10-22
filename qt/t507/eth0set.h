#ifndef ETH0SET_H
#define ETH0SET_H

#include <QWidget>
#include <QString>
#include <QProcess>
#include <QDebug>
#include <unistd.h>
#include "eth0_popup.h"
#include <QTimer>

namespace Ui {
class eth0set;
}

class eth0set : public QWidget
{
    Q_OBJECT

public:
    explicit eth0set(QWidget *parent = 0);
    ~eth0set();

    eth0_popup popup;
    void language_reload();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();
    void speedtest();
    void on_pushButton_9_clicked();
    void increaseip(QString,QString,QString);
    void modifyip(QString,QString,QString);
    void deleteip(QString);

signals:
    void Mysignal();
    void StartTestSpeed();
private:
    Ui::eth0set *ui;

};

#endif // ETH0SET_H

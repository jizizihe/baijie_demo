#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <QMainWindow>
#include "bluetooth_thread.h"

#include <QMainWindow>
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QProcess>
#include <QTextEdit>
#include <QComboBox>
#include <QThread>
#include <QFile>
#include<iostream>

namespace Ui {
class bluetooth;
}

class bluetooth : public QMainWindow
{
    Q_OBJECT

public:
    explicit bluetooth(QWidget *parent = 0);
    ~bluetooth();

private slots:
    void retBt_clicked();
    void BTScanBt_clicked();
    void BTConnectBt_clicked();
    void BTPairBt_clicked();
    void recvmsg(QString);
    void setText_slot(bool);

signals:
    void Mysignal();
    void bluetooth_signal(int signal_type,QString BtAddress);//signal_type:1--scan;2--pair;3--connect

private:
    Ui::bluetooth *ui;

    QComboBox * BTNameBox;
    QTextEdit * BTText;
    QLabel * BTTitleLab;
    QPushButton * BTScanBt;
    QPushButton * BTConnectBt;
    QPushButton * BTPairBt;
    QPushButton * retBt;

    bluetooth_thread * BtThread=new bluetooth_thread(NULL);//声明线程

};

#endif // BLUETOOTH_H

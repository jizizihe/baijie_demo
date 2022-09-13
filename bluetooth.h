#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <QMainWindow>
#include "bluetooth_thread.h"
#include "wifi_bt_interface.h"
#include "database.h"

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
#include <QListWidget>
#include <QScrollBar>
#include <QMessageBox>
#include <QMovie>
#include<QScreen>

namespace Ui {
class bluetooth;
}

class bluetooth : public QMainWindow
{
    Q_OBJECT

public:
    explicit bluetooth(QWidget *parent = 0);
    ~bluetooth();

    void language_reload();
    void blue_font();
    void bluetooth_sql();
    void showEvent(QShowEvent *event);
    QStringList BtPairList;

private slots:
    void recv_msg(int,QString);

    void BtnChange_flag(bool);

    void on_BTScanBtn_clicked();

    void on_BTConnectBtn_clicked();

    void on_retBtn_clicked();

    void on_pushButton_clicked();

    void on_BtNameListWidget_itemClicked(QListWidgetItem *item);

    void on_Bt_pairedListwidget_itemClicked(QListWidgetItem *item);

    void connect_refresh();

    void on_btn_status_clicked();

    void on_btn_disconnect_clicked();

    void on_btn_remove_clicked();

signals:
    void bluetooth_scan_msg();
    void bluetooth_pair_msg(QString);
    void bluetooth_connect_msg(QString);
    void bluetooth_disscan();
    void Mysignal();
    void rm_shell();
    //void bluetooth_signal(int signal_type,QString BtAddress);//signal_type:1--scan;2--pair;3--connect

private:
    Ui::bluetooth *ui;

    QStringList BtScanList;
    QLabel *LoadLabel;
    QMovie *pMovie;

    QThread * myThread;
    bluetooth_thread * BluetoothThread;
    wifi_bt_interface * wifi_bt_t;
    database database_w;
};

#endif // BLUETOOTH_H

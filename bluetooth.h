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
    void languageReload();
    void bluetoothFont();
    void bluetoothListShow(QString,QString);
    void bluetoothConnectStatus();
    void switchSetText();
    void showEvent(QShowEvent *event);

private slots:
    void recv_msg(int,QString);
    void on_btn_btScan_clicked();
    void on_btn_ret_clicked();
    void on_BtNameListWidget_itemClicked(QListWidgetItem *item);
    void connect_refresh();
    void on_btn_disconnect_clicked();
    void on_btn_remove_clicked();
    void scan_refresh();
    void switch_change_flag(bool);

signals:
    void bluetooth_scan_msg();
    void bluetooth_pair_msg(QString);
    void bluetooth_connect_msg(QString);
    void bluetooth_back_msg();
    void rm_shell_msg();

private:
    Ui::bluetooth *ui;
    QStringList btScanList;
    QLabel *loadLabel;
    QMovie *pMovie;
    QThread * myThread;
    bluetooth_thread * bluetoothThread;
    QThread * blutooth;
    wifi_bt_interface * wifi_bt_interfaceWg;
    database databaseWg;
    QStringList btPairList;
};

#endif // BLUETOOTH_H

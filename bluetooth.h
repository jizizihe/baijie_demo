#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <QMainWindow>
#include "bluetooth_thread.h"
#include "bluetooth_interface.h"
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
    void setBluetoothFont();
    void bluetoothListShow(QString,QString);
    void bluetoothConnectStatus();
    void setSwitchText();
    void showEvent(QShowEvent *event);

private slots:
    void recv_msg(int,QString);
    void on_btn_btScan_clicked();
    void on_btn_ret_clicked();
    void on_BtNameListWidget_itemClicked(QListWidgetItem *item);
    void on_btn_disconnect_clicked();
    void on_btn_remove_clicked();
    void connect_info_refresh();
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
    QStringList g_btScanList;
    QLabel *g_loadLabel;
    QMovie *g_pMovie;
    QThread * g_myThread;
    bluetooth_thread * g_bluetoothThread;
    bluetooth_interface * g_bluetoothInterface;
    database g_database;
    QStringList g_btPairList;
};

#endif // BLUETOOTH_H

#ifndef WIFICONNECT_H
#define WIFICONNECT_H

#include <QMainWindow>
#include <QComboBox>
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QProcess>
#include <QScrollBar>
#include <QListWidget>
#include <QThread>
#include <pthread.h>

QString wifi_scan();
void wifi_connect(QString WifiSsid,QString PassWd);

namespace Ui {
class WifiConnect;
}

class WifiConnect : public QMainWindow
{
    Q_OBJECT

public:
    explicit WifiConnect(QWidget *parent = 0);
    ~WifiConnect();

//    static void *scan_thread(void *);
//    void WifiScanThread();
    void wifi_connect_show();

    void language_reload();

private slots:
    void WifiScanBt_clicked();
    void WifiCleanBt_clicked();
    void WifiConnectBt_clicked();
    void WifiCloseBt_clicked();
    void ListWidgeItem_clicked();
    void scan_show();

private:
    Ui::WifiConnect *ui;

    QLabel * WifiSsidLab;
    QLabel * WifiPasswdLab;
    QLineEdit *WifiPasswdLine;
    QLineEdit *WifiSsidLine;
    QComboBox * WifiNameBox;
    QPushButton * WifiScanBt;
    QPushButton * WifiCleanBt;
    QPushButton * WifiConnectBt;
    QPushButton * WifiCloseBt;
    QListWidget * WifiScanListWid;
};

#endif // WIFICONNECT_H

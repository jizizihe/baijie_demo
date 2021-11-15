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
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QMessageBox>
#include <QMovie>
#include<QScreen>
#include "wifi_thread.h"

void wifi_connect(QString WifiSsid,QString PassWd);

namespace Ui {
class WifiConnect;
}

class slideButton : public QWidget
{
    Q_OBJECT

public:
    explicit slideButton(QWidget *parent = 0);
    void mouseReleaseEvent(QMouseEvent *);  //mouse up time
    int switchflag = 0;
    int initflag = 0;

    int get_switchflag()
    {
        return switchflag;
    }

protected:
    void paintEvent(QPaintEvent *event);

signals:
    void buttonChange(int );

public slots:
};



class WifiConnect : public QMainWindow
{
    Q_OBJECT

public:
    explicit WifiConnect(QWidget *parent = 0);
    ~WifiConnect();
    void language_reload();

signals:
    void ToThread(); // 信号
    void wifi_scan_msg();

private slots:
    void WifiScanBt_clicked();
    void WifiCleanBt_clicked();
    void WifiConnectBt_clicked();
    void WifiCloseBt_clicked();
    void ListWidgeItem_clicked();
    void BtnChange_flag(int switchflag);

    void recv_msg(QString );

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

    QStringList scanlist;

    QLabel *LoadLabel;
    QMovie *pMovie;

    slideButton *SwitchBtn;
};




#endif // WIFICONNECT_H

#ifndef WIFI_H
#define WIFI_H

#include <QMainWindow>
#include <QLabel>
#include <QFont>
#include <QPushButton>
#include <QTextEdit>
#include <QProcess>
#include <QDebug>
#include <string>
#include "wificonnect.h"
#include "hotspot.h"


namespace Ui {
class wifi;
}

class wifi : public QMainWindow
{
    Q_OBJECT

public:
    explicit wifi(QWidget *parent = 0);
    ~wifi();

signals:
    void Mysignal();

private slots:
    void retBt_clicked();
    void WifiConnectBt_clicked();
    void WifiDisconnectBt_clicked();
    void HotSpotBt_clicked();
    void SignalQualityBt_clicked();
    void StatusBt_clicked();

private:
    Ui::wifi *ui;

    QPushButton *retBt;
    QLabel * pLabel;
    QPushButton * WifiConnectBt;
    QPushButton * WifiDisconnectBt;
    QPushButton * HotSpotBt;
    QPushButton * SignalQualityBt;
    QPushButton * StatusBt;

    QTextEdit * WifiMsgText;

};

#endif // WIFI_H

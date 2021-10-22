#ifndef WIFICONNECT_H
#define WIFICONNECT_H

#include <QMainWindow>
#include <QComboBox>
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QProcess>

void wifi_scan();
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

    void language_reload();

private slots:
    void WifiScanBt_clicked();
    void WifiCleanBt_clicked();
    void WifiConnectBt_clicked();
    void WifiCloseBt_clicked();

private:
    Ui::WifiConnect *ui;

    QLabel * WifiSsidLab;
    QLabel * WifiPasswdLab;
    QLineEdit *WifiPasswdLine;
    QComboBox * WifiNameBox;
    QPushButton * WifiScanBt;
    QPushButton * WifiCleanBt;
    QPushButton * WifiConnectBt;
    QPushButton * WifiCloseBt;

};

#endif // WIFICONNECT_H

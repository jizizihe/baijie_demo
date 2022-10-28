#ifndef WIFICONDIALOG_H
#define WIFICONDIALOG_H

#include "wifi_bt_interface.h"
#include <QDialog>
#include <QMessageBox>
#include <QDebug>
#include "database.h"

namespace Ui {
class WifiConDialog;
}

class WifiConDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WifiConDialog(QWidget *parent = 0);
    ~WifiConDialog();

    void languageReload();
    void setWifiNameText(QString);
    void setPasswdText(QString );
    QString getWifiNameText();
    QString getPasswdText();
    QString GetWifiOkBtnText();
    void setWifiOkBtnText(int);
    void wifidialFont();

signals:
    void wifi_connect_dialog_msg(QString,QString);
    void wifi_info_fresh_msg(QString);
    void wifidial_close_msg();
    void wifi_show_refresh_msg();
    void wifi_modify_pass_msg(QString,QString);

private slots:
    void on_btn_ok_clicked();
    void on_btn_cancel_clicked();
    void wifi_wait_end_func();

public slots:
    void wifi_modify_pass(bool);

private:
    Ui::WifiConDialog *ui;
    QLabel *WifiLoadLabel;
    QMovie *WifiMovie;
    database databaseWg;
};

#endif // WIFICONDIALOG_H

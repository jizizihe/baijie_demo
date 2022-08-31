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

    bool event(QEvent *event);

    void language_reload();

    void SetWifiNameText(QString );
    void SetPasswdText(QString );
    QString GetWifiNameText();
    QString GetPasswdText();

    QString GetWifiOkBtnText();
    void SetWifiOkBtnText(QString);
    void wifidial_font();

signals:
    void wifi_connect_dialog_signal(QString,QString);
    void wifi_info_fresh_msg(QString);
    void wifidial_close();

private slots:
    void on_WifiOkBtn_clicked();

    void on_WifiCancelBtn_clicked();

    void wifi_wait_end_func();

private:
    Ui::WifiConDialog *ui;

    wifi_bt_interface * wifi_bt_t;

    QLabel *WifiLoadLabel;
    QMovie *WifiMovie;
    database database_w;
};

#endif // WIFICONDIALOG_H

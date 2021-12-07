#ifndef WIFICONDIALOG_H
#define WIFICONDIALOG_H

#include "wifi_bt_interface.h"

#include <QDialog>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class WifiConDialog;
}

class WifiConDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WifiConDialog(QWidget *parent = 0);
    ~WifiConDialog();

    void language_reload();

    void SetWifiNameText(QString );
    void SetPasswdText(QString );
    QString GetWifiOkBtnText();
    void SetWifiOkBtnText(QString);

signals:
    void wifi_connect_dialog_signal(QString,QString);

private slots:
    void on_WifiOkBtn_clicked();

    void on_WifiCancelBtn_clicked();

private:
    Ui::WifiConDialog *ui;

    wifi_bt_interface * wifi_bt_t;
};

#endif // WIFICONDIALOG_H

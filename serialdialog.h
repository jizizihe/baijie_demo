#ifndef SERIALDIALOG_H
#define SERIALDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDebug>
#include <QMessageBox>
#include "database.h"

typedef struct SERIALCONFIG
{
    int index;
    int count;
    int checked_id[64];
    QString mode;
    QString checkedName;
    QList<QAbstractButton*> checkedBtnList;
}serial_config;

namespace Ui {
class serialdialog;
}

class serialdialog : public QDialog
{
    Q_OBJECT

public:
    explicit serialdialog(QWidget *parent = 0);
    ~serialdialog();
    Ui::serialdialog *ui;
    QButtonGroup* pButtonGroup;

    void getSerialCheckedName();
    void languageReload();

signals:
    void serial_config_msg(serial_config);
    void serial_test_msg();
    void serial_dialog_stop_msg();
private slots:
    void on_btn_serialCancel_clicked();
    void pButtonGroup_pressed_func(int);
    void on_btn_serialOk_clicked();
    void on_btn_serialCheckAll_clicked();
    void on_serialModeBox_currentIndexChanged(const QString &arg1);

private:
    serial_config serialConfig;
    database databaseWg;
};

#endif // SERIALDIALOG_H

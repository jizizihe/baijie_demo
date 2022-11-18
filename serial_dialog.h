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

typedef struct SertalConfig
{
    int index;
    int count;
    int checkedId[64];
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
    void getSerialCheckedName();
    void languageReload();

signals:
    void serial_config_msg(serial_config);
    void serial_test_msg();
    void serial_dialog_stop_msg();
private slots:
    void on_btn_serialCancel_clicked();
    void btn_group_pressed_func(int);
    void on_btn_serialOk_clicked();
    void on_btn_serialCheckAll_clicked();
    void on_serialModeBox_currentIndexChanged(const QString &arg1);

private:
    serial_config g_serialConfig;
    database g_database;
    QButtonGroup* g_pButtonGroup;
};

#endif // SERIALDIALOG_H

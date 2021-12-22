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

    QString getSerialCheckedName();

signals:
    void serial_config_msg(serial_config);
private slots:

    void on_serialCancelBtn_clicked();

    void on_serialOkBtn_clicked();

    void on_serialCheckAllBtn_clicked();


private:

    serial_config serialConfig;

};

#endif // SERIALDIALOG_H

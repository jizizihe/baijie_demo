#ifndef SERIAL_SET_PORT2_H
#define SERIAL_SET_PORT2_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>

namespace Ui {
class serial_set_port2;
}

class serial_set_port2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit serial_set_port2(QWidget *parent = 0);
    ~serial_set_port2();
    QStringList getPortNameList();
    void language_reload();
    void serial_set2_font();

signals:
    void port2_set(QString,QString,int,QString);
    void port2_re();

private slots:
    void on_OK_Btn_clicked();

    void on_SendBtn_2_clicked();

private:
    Ui::serial_set_port2 *ui;
    QStringList m_portNameList;
};

#endif // SERIAL_SET_PORT2_H

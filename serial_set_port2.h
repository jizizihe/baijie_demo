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

signals:

private slots:


private:
    Ui::serial_set_port2 *ui;

};

#endif // SERIAL_SET_PORT2_H

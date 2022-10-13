#ifndef SERIAL_SET_H
#define SERIAL_SET_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>

namespace Ui {
class serial_set;
}

class serial_set : public QMainWindow
{
    Q_OBJECT

public:
    explicit serial_set(QWidget *parent = 0);
    ~serial_set();

private slots:

signals:
private:
    Ui::serial_set *ui;
    QStringList m_portNameList;
};

#endif // SERIAL_SET_H

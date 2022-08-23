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
    QStringList getPortNameList();
     void language_reload();
     void serial_set_font();
     int hide_flag;

private slots:
    void on_OpenBtn_clicked();

    void on_SendBtn_clicked();

signals:
    void port1_set(QString,QString,int,QString);
    void port1_re();

private:
    Ui::serial_set *ui;
    QStringList m_portNameList;
};

#endif // SERIAL_SET_H

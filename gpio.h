#ifndef GPIO_H
#define GPIO_H

#include <QWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QDebug>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QComboBox>
#include <QGroupBox>
#include <QValidator>
#include "gpio_interface.h"
#include <QScrollBar>

namespace Ui {
class gpio;
}

class gpio : public QWidget
{
    Q_OBJECT

public:
    explicit gpio(QWidget *parent = 0);
    ~gpio();
    void languageReload();
    void gpioFont();
    void setTextStatusSwitch(int);
    void setTextValueSwitch(int);
    void showEvent(QShowEvent *event);
    bool isTruePort(QString,int i);           //Check whether the port mode is correct
    bool isEnglish(QString &qstrSrc);
    bool isNumber(QString &qstrSrc);

private slots:
    void ret_clicked();
    void srceen_clear();    
    bool warning();
    void on_btn_hint_clicked();
    void status_switch_change_flag(bool flag);
    void value_switch_change_flag(bool flag);
    void gpio_refresh();

signals:
    void gpio_back_msg();

private:
    Ui::gpio *ui;
    int portNumInt[384] = {0};  //For example: 205
    char portNumStr[384][4];    //For example: g13
    int num = 0,portCount = 0;
    struct occupiedGpioStr occupiedGpio;
};

#endif // GPIO_H

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
    void setGpioFont();
    void setTextStatusSwitch(int);
    void setTextValueSwitch(int);
    void showEvent(QShowEvent *event);
    bool isTruePort(QString,int i);           // Check whether the port mode is correct
    bool isEnglish(QString &qstrSrc);
    bool isNumber(QString &qstrSrc);

private slots:
    void btn_ret_clicked();
    void srceen_clear();    
    void on_btn_hint_clicked();
    void status_switch_change_flag(bool flag);
    void value_switch_change_flag(bool flag);
    void gpio_refresh();
    bool input_warning();            // Determine whether the input complies with the rules

signals:
    void gpio_back_msg();

private:
    Ui::gpio *ui;
    int g_portNumInt[384] = {0};      // For example: 205
    char g_portNumStr[384][4];        // For example: g13
    int g_portNum = 0,g_portCount = 0;
    struct OccupiedGpioStr g_occupiedGpio;
};

#endif // GPIO_H

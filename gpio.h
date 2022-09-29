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
#include "input_method.h"

namespace Ui {
class gpio;
}

class gpio : public QWidget
{
    Q_OBJECT

public:
    explicit gpio(QWidget *parent = 0);
    ~gpio();
    void key_show();
    void key_hide();
    void language_reload();
    void gpio_font();
    void settext_statusbtn(int);
    void settext_valuebtn(int);
    void showEvent(QShowEvent *event);

private slots:
    void ret_clicked();
    void srceenclear();
    bool isEnglish(QString &qstrSrc);
    bool isNumber(QString &qstrSrc);
    bool istrueport(QString,int i);
    bool warning();
    void on_pushButton_clicked();
    void BtnChange_flag1(bool flag);
    void BtnChange_flag2(bool flag);
    void gpio_refresh();

signals:
    void Mysignal();

private:
    Ui::gpio *ui;
    QGroupBox *stateGroup;
    QGroupBox *valueGroup;
    QPushButton *ret;

    QTextEdit *display;
    QRadioButton *rBtnhigh;
    QRadioButton *rBtnlow ;
    QRadioButton *rBtnout;
    QRadioButton *rBtnin ;
    input_method input_methodw;
    int port_num[384] = {0};
    char portnum[384][4];
    int num = 0,count = 0;
    struct occupied_gpio_s occupied_gpio;

};

#endif // GPIO_H

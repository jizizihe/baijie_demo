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

    void language_reload();

private slots:
    void rBtnout_clicked();
    void rBtnin_clicked();
    void ret_clicked();
    void srceenclear();
    bool isEnglish(QString &qstrSrc);
    bool isNumber(QString &qstrSrc);
    bool istrueport(QString,int i);
    void rBtnhigh_clicked();
    void rBtnlow_clicked();
    bool warning();

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
    int port_num[384] = {0};
    char portnum[384][4];
    int num = 0,count = 0;
    struct occupied_gpio_s occupied_gpio;
};

#endif // GPIO_H

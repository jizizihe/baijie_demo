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

namespace Ui {
class gpio;
}

class gpio : public QWidget
{
    Q_OBJECT

public:
    explicit gpio(QWidget *parent = 0);
    ~gpio();

private slots:
    void on_pushButton_clicked();
    void on_lineedit1_1_editingFinished();
    void rBtnout_clicked();
    void rBtnin_clicked();
    void rBtnhigh_clicked();
    void rBtnlow_clicked();
    void ret_clicked();

signals:
    void Mysignal();
private:
    Ui::gpio *ui;
    QTextEdit *display;
    char *gpio_state;
    int gpio_val;
    int port_num;
};

#endif // GPIO_H

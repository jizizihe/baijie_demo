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
    void on_pushButton_clicked();
    void on_lineedit1_1_editingFinished();
    void rBtnout_clicked();
    void rBtnin_clicked();
    void ret_clicked();
    void srceenclear();
    void on_pushButton_2_clicked();

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
    int port_num;
};

#endif // GPIO_H

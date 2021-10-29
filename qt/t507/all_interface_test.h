#ifndef ALL_INTERFACE_TEST_H
#define ALL_INTERFACE_TEST_H

#include <QMainWindow>
#include <QTimer>


void camera_interface();

namespace Ui {
class all_interface_test;
}

class all_interface_test : public QMainWindow
{
    Q_OBJECT

public:
    explicit all_interface_test(QWidget *parent = 0);
    ~all_interface_test();

private slots:
    void on_return_2_clicked();

    void on_test_clicked();

    void on_clear_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

signals:

    void Mysignal();

private:
    Ui::all_interface_test *ui;
};

#endif // ALL_INTERFACE_TEST_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QString>
#include <QRadioButton>
#include <QGroupBox>




namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    //void on_lineEdit_textChanged(const QString &arg1);
    void on_lineedit1_1_editingFinished();
    void on_rBtnout_clicked();
    void on_rBtnin_clicked();
    void on_value_currentTextChanged(const QString &arg1);
    //void on_comboBox_currentTextChanged(const QString &arg1);

    /*void on_radioButton_clicked(bool checked);

    void on_radioButton_2_clicked(bool checked);
*/
private:
    Ui::MainWindow *ui;
    QGroupBox *genderGroup = new QGroupBox(this);
    QRadioButton *rBtnout = new QRadioButton("out", genderGroup);
    QRadioButton *rBtnin = new QRadioButton("in", genderGroup);
    QString gpio_state;
    QString gpio_val;

};


#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPushButton>
#include <QDebug>
#include <QLineEdit>
#include <QDialog>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QComboBox>

extern "C"
{
    #include "gpio_set.h"
}
extern int	calc_port_num(char port, int num);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QLineEdit *edit = new QLineEdit(this);

    QGroupBox *genderGroup = new QGroupBox(this);
    genderGroup->setTitle("state");
    genderGroup->setGeometry(200, 220, 200, 150);

    QRadioButton *rBtnout = new QRadioButton("out", genderGroup);
    QRadioButton *rBtnin = new QRadioButton("in", genderGroup);
    rBtnout->setChecked(true);

    QVBoxLayout *genderLayout = new QVBoxLayout(genderGroup);
    genderLayout->addWidget(rBtnout);
    genderLayout->addWidget(rBtnin);

    genderGroup->setLayout(genderLayout);

    QLabel *Label_value = new QLabel(this);
    Label_value->setText("If chosed out,please setting the value");
    Label_value->move(500,220);


    QComboBox *value = new QComboBox(this);
    value->move(500,270);
    //value->setEditable(true);
    value->addItem("high");
    value->addItem("low");

    QVBoxLayout *Label_layout = new QVBoxLayout();
    Label_layout->addWidget(Label_value);



    //connect(lineedit1_1, SIGNAL(returnPressed()), this, SLOT(on_lineedit1_1_editingFinished()));
    connect(rBtnin,SIGNAL(clicked(bool)),this,SLOT(on_rBtnin_clicked()));
    connect(rBtnout,SIGNAL(clicked(bool)),this,SLOT(on_rBtnout_clicked()));

}

MainWindow::~MainWindow()
{

    delete ui;

}

void MainWindow::on_rBtnin_clicked()
{
    gpio_state = "in";
}

void MainWindow::on_rBtnout_clicked()
{
    gpio_state = "out";

}

void MainWindow::on_value_currentTextChanged(const QString &arg1)
{
    gpio_val = arg1;
}

void MainWindow::on_pushButton_clicked()
{/*
    QString str = ui->comboBox->currentText();
    qDebug() << str;

*/
        //qDebug() << rBtnout->clicked();
        qDebug() << gpio_state;
        qDebug() << gpio_val;
}

void MainWindow::on_lineedit1_1_editingFinished()
{
    QLineEdit* button = (QLineEdit*)sender();

    QString str = button->text();

    if((str.size() < 2 || str.size() > 4) && str.size() != 0)
    {
        QMessageBox::information(NULL, NULL, "plz input true GPIO!", QMessageBox::Ok);
        return;
    }

    QString port = str.mid(0,1);
    QByteArray temp = port.toLatin1();
    char gpio_port = *(temp.data());
    if(gpio_port == 'p' || gpio_port == 'P')
    {
        port = str.mid(1,2);
        temp = port.toLatin1();
        gpio_port = *(temp.data());

        QString num = str.mid(2);
        int gpio_num = num.toInt();
        int port_num = calc_port_num(gpio_port, gpio_num);

        if(port_num < 0 && str.size() != 0)
        {
            QMessageBox::information(NULL, NULL, "Please input true GPIO!", QMessageBox::Ok);
            qDebug() << "input error GPIO port:" << port_num;
            return;
        }
        qDebug() << port_num;
    }
    else
    {
        QString num = str.mid(1);
        int gpio_num = num.toInt();
        int port_num = calc_port_num(gpio_port, gpio_num);

        if(port_num < 0 && str.size() != 0)
        {
            QMessageBox::information(NULL, NULL, "Please input true GPIO!", QMessageBox::Ok);
            qDebug() << "input error GPIO port:" << port_num;
            return;
        }
        qDebug() << port_num;
    }


    /*
    if(str.size() < 2 || str.size() > 4)
    {
        QMessageBox::information(NULL, NULL, "plz input true GPIO!", QMessageBox::Ok);
        return;
    }

    QString sign = str.mid(0,1);
    QByteArray temp = sign.toLatin1();
    char b=*(temp.data());

    QString num = str.mid(1);
    int c=num.toInt();
    int d = calc_port_num(b,c);

    if (d < 0)
    {
        QMessageBox::information(NULL, NULL, "plz input true GPIO!", QMessageBox::Ok);
        return;
    }

    QMessageBox mess(QMessageBox::Information,tr(""), tr("What do you want to do with this GPIO port?"));
    QPushButton *button1= (mess.addButton(tr("in"), QMessageBox::AcceptRole));
    QPushButton *button2= (mess.addButton(tr("out"), QMessageBox::YesRole));
    QPushButton *buttonCancel = (mess.addButton(tr("cancel"), QMessageBox::RejectRole));
    mess.exec();

    if(mess.clickedButton() == button1)
    {
        if(gpio_in(b,c) == 0)
        {
            QMessageBox::information(NULL, NULL, "GPIO port is low.", QMessageBox::Ok);
        }
        else
        {
            QMessageBox::information(NULL, NULL, "GPIO port is high.", QMessageBox::Ok);
        }
    }
    else if(mess.clickedButton() == button2)
    {
        QMessageBox mess(QMessageBox::Information,tr(""), tr("Do you want to pull up or down this GPIO port?"));
        QPushButton *button1= (mess.addButton(tr("up"), QMessageBox::AcceptRole));
        QPushButton *button2= (mess.addButton(tr("down"), QMessageBox::YesRole));
        QPushButton *buttonCancel = (mess.addButton(tr("cancel"), QMessageBox::RejectRole));
        mess.exec();

        if(mess.clickedButton() == button1)
        {
            gpio_out(b,c,1);
            QMessageBox::about(NULL, "About", "This GPIO port has been set to high.");
        }
        else if(mess.clickedButton() == button2)
        {
            gpio_out(b,c,0);
            QMessageBox::about(NULL, "About", "This GPIO port has been set to low.");
        }
        else
        {
            return;
        }
    }
    else
    {
        return;
    }
*/



}

/*
void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    QString value = ui->comboBox->currentText();
    qDebug() << "str:" << value;
}
*/



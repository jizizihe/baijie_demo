#include "gpio.h"
#include "ui_gpio.h"
extern "C"
{
    #include "gpio_interface.h"
}

gpio::gpio(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gpio)
{
    ui->setupUi(this);

    display = new QTextEdit(this);
    display->setReadOnly(true);
    display->resize(330,320);
    display->move(480,130);

   QGroupBox *stateGroup = new QGroupBox(this);
   stateGroup->setTitle("state");
   stateGroup->setGeometry(200, 120, 200, 150);

   QRadioButton *rBtnout = new QRadioButton("out", stateGroup);
   QRadioButton *rBtnin = new QRadioButton("in", stateGroup);


   QVBoxLayout *stateLayout = new QVBoxLayout(stateGroup);
   stateLayout->addWidget(rBtnout);
   stateLayout->addWidget(rBtnin);

   stateGroup->setLayout(stateLayout);

   QGroupBox *valueGroup = new QGroupBox(this);
   valueGroup->setTitle("value");
   valueGroup->setGeometry(200, 300, 200, 150);

   QRadioButton *rBtnhigh = new QRadioButton("high", valueGroup);
   QRadioButton *rBtnlow = new QRadioButton("low", valueGroup);

   QVBoxLayout *valueLayout = new QVBoxLayout(valueGroup);
   valueLayout->addWidget(rBtnhigh);
   valueLayout->addWidget(rBtnlow);

   QPushButton *ret = new QPushButton(this);
   ret->setFixedSize(100,40);
   ret->setText("return");
   ret->move(10,10);

   connect(rBtnin,SIGNAL(clicked(bool)),this,SLOT(rBtnin_clicked()));
   connect(rBtnout,SIGNAL(clicked(bool)),this,SLOT(rBtnout_clicked()));
   connect(rBtnhigh,SIGNAL(clicked(bool)),this,SLOT(rBtnhigh_clicked()));
   connect(rBtnlow,SIGNAL(clicked(bool)),this,SLOT(rBtnlow_clicked()));
   connect(ret,SIGNAL(clicked(bool)),this,SLOT(ret_clicked()));
}

gpio::~gpio()
{
    delete ui;
}

void gpio::ret_clicked()
{
    emit Mysignal();
}

void gpio::rBtnin_clicked()
{
    gpio_state = "in";
}

void gpio::rBtnout_clicked()
{
    gpio_state = "out";

}

void gpio::rBtnhigh_clicked()
{
    gpio_val = 1;
}

void gpio::rBtnlow_clicked()
{
    gpio_val = 0;

}

void gpio::on_pushButton_clicked()
{
    int value;

    display->clear();

    gpio_export(port_num);
    gpio_set_state(port_num, gpio_state);
    value = gpio_get_value(port_num);
//    qDebug() << value;
    if(gpio_state == "in")
    {
        display->insertPlainText("gpio_port: ");
        display->insertPlainText(ui->lineedit1_1->text());
        display->append("state: ");
        display->insertPlainText("in");
        display->append("value: ");
        if(value == 1)
        {
            display->insertPlainText("high");
        }
        else if(value == 0)
        {
            display->insertPlainText("low");
        }

    }
    else if(gpio_state == "out")
    {
        gpio_set_value(port_num, gpio_val);
        display->insertPlainText("gpio_port: ");
        display->insertPlainText(ui->lineedit1_1->text());
        display->append("state: ");
        display->insertPlainText("out");
        display->append("value: ");
        if(gpio_val == 1)
        {
            display->insertPlainText("high");
        }
        else if(gpio_val == 0)
        {
            display->insertPlainText("low");
        }
    }

}

void gpio::on_lineedit1_1_editingFinished()
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
        port_num = calc_port_num(gpio_port, gpio_num);

        if(port_num < 0 && str.size() != 0)
        {
            QMessageBox::information(NULL, NULL, "Please input true GPIO!", QMessageBox::Ok);
            qDebug() << "input error GPIO port:" << port_num;
            return;
        }

    }
    else
    {
        QString num = str.mid(1);
        int gpio_num = num.toInt();
        port_num = calc_port_num(gpio_port, gpio_num);

        if(port_num < 0 && str.size() != 0)
        {
            QMessageBox::information(NULL, NULL, "Please input true GPIO!", QMessageBox::Ok);
            qDebug() << "input error GPIO port:" << port_num;
            return;
        }

    }

}

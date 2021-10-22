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

    QRegExp regx("[a-zA-Z0-9]{4}$");  //表示只能输入数字和字母
    QValidator *validator = new QRegExpValidator(regx, ui->lineedit1_1);
    ui->lineedit1_1->setValidator( validator );


    display = new QTextEdit(this);
    display->setReadOnly(true);
    display->resize(330,320);
    display->move(480,130);

    stateGroup = new QGroupBox(this);
    stateGroup->setTitle(tr("state"));
    stateGroup->setGeometry(200, 120, 200, 150);

    rBtnout = new QRadioButton(tr("out"), stateGroup);
    rBtnin = new QRadioButton(tr("in"), stateGroup);

    QVBoxLayout *stateLayout = new QVBoxLayout(stateGroup);
    stateLayout->addWidget(rBtnout);
    stateLayout->addWidget(rBtnin);

    stateGroup->setLayout(stateLayout);

    valueGroup = new QGroupBox(this);
    valueGroup->setTitle(tr("value"));
    valueGroup->setGeometry(200, 300, 200, 150);

    rBtnhigh = new QRadioButton(tr("high"), valueGroup);
    rBtnlow = new QRadioButton(tr("low"), valueGroup);

    QVBoxLayout *valueLayout = new QVBoxLayout(valueGroup);
    valueLayout->addWidget(rBtnhigh);
    valueLayout->addWidget(rBtnlow);

    ret = new QPushButton(this);
    ret->setFixedSize(100,40);
    ret->setText(tr("return"));
    ret->move(10,10);

    connect(rBtnin,SIGNAL(clicked(bool)),this,SLOT(rBtnin_clicked()));
    connect(rBtnout,SIGNAL(clicked(bool)),this,SLOT(rBtnout_clicked()));
    connect(ret,SIGNAL(clicked(bool)),this,SLOT(ret_clicked()));
    connect(this,SIGNAL(Mysignal()),this,SLOT(srceenclear()));
}

gpio::~gpio()
{
    delete ui;
}

void gpio::ret_clicked()
{
    emit Mysignal();
}

void gpio::srceenclear()
{
    display->clear();
    ui->lineedit1_1->clear();
    rBtnout->setCheckable(false);
    rBtnin->setCheckable(false);
    rBtnhigh->setCheckable(false);
    rBtnlow->setCheckable(false);
    rBtnout->setCheckable(true);
    rBtnin->setCheckable(true);
    rBtnhigh->setCheckable(true);
    rBtnlow->setCheckable(true);
    rBtnhigh->setEnabled(true);
    rBtnlow->setEnabled(true);
}

void gpio::rBtnin_clicked()
{
    rBtnhigh->setDisabled(true);
    rBtnlow->setDisabled(true);
    rBtnhigh->setCheckable(false);
    rBtnlow->setCheckable(false);

}

void gpio::rBtnout_clicked()
{
    rBtnhigh->setEnabled(true);
    rBtnlow->setEnabled(true);
    rBtnhigh->setCheckable(true);
    rBtnlow->setCheckable(true);

}

void gpio::on_pushButton_clicked()
{
    display->clear();
    gpio_export(port_num);

    if(ui->lineedit1_1->text().isEmpty())
    {
        display->append(QString(tr("The GPIO port entered is empty!")));
        return;
    }
    if(rBtnin->isChecked())
    {
        gpio_set_state(port_num, "in");
//        display->insertPlainText("gpio_port: ");
//        display->insertPlainText(ui->lineedit1_1->text());

        display->append(QString(tr("gpio_port: %1")).arg(ui->lineedit1_1->text()));

//        display->append("state: ");
//        display->insertPlainText("in");

        display->append(QString(tr("state: %1")).arg(tr("in")));

//        display->append("value: ");
//        display->insertPlainText( QString("%1").arg((gpio_get_value(port_num))));

        display->append( QString(tr("value: %1")).arg((gpio_get_value(port_num))));
    }
    else if(rBtnout->isChecked())
    {
        gpio_set_state(port_num, "out");
//        display->insertPlainText("gpio_port: ");
//        display->insertPlainText(ui->lineedit1_1->text());

        display->append(QString(tr("gpio_port: %1")).arg(ui->lineedit1_1->text()));

//        display->append("state: ");
//        display->insertPlainText("out");

        display->append(QString(tr("state: %1")).arg(tr("out")));

//        display->append("value: ");
        if(rBtnhigh->isChecked())
        {
            gpio_set_value(port_num, 1);
//            display->insertPlainText("high");

            display->append(QString(tr("value: %1")).arg(tr("high")));
        }
        else if(rBtnlow->isChecked())
        {
            gpio_set_value(port_num, 0);
//            display->insertPlainText("low");

            display->append(QString(tr("value: %1")).arg(tr("low")));
        }
    }
    gpio_unexport(port_num);

}

void gpio::on_lineedit1_1_editingFinished()
{
    //QLineEdit* button = (QLineEdit*)sender();

    QString str = ui->lineedit1_1->text();

    if(str.size() < 2 && str.size() != 0)
    {
        QMessageBox::information(NULL, NULL, tr("Please input true GPIO!"), QMessageBox::Ok);
        return;
    }

    QString port = str.mid(0,1);
    QByteArray temp = port.toLatin1();
    char gpio_port = *(temp.data());
    if(gpio_port == 'p' || gpio_port == 'P')
    {
        QString tmp = str.at(2);
        QByteArray ba = tmp.toLatin1();
        char* s = ba.data();
        while (*s && *s >= '0' && *s <= '9') s++;
        if (*s)
        {
           QMessageBox::information(NULL, NULL, tr("Please input true GPIO!"), QMessageBox::Ok);
           return;
        }
        port = str.mid(1,2);
        temp = port.toLatin1();
        gpio_port = *(temp.data());

        QString num = str.mid(2);
        int gpio_num = num.toInt();
        port_num = calc_port_num(gpio_port, gpio_num);

        if(port_num < 0 && str.size() != 0)
        {
            QMessageBox::information(NULL, NULL, tr("Please input true GPIO!"), QMessageBox::Ok);
            qDebug() << "input error GPIO port:" << port_num;
            return;
        }

    }
    else
    {
        QString tmp = str.at(1);
        QByteArray ba = tmp.toLatin1();
        char* s = ba.data();
        while (*s && *s >= '0' && *s <= '9') s++;
        if (*s)
         {
            QMessageBox::information(NULL, NULL, tr("Please input true GPIO!"), QMessageBox::Ok);
            return;
         }
        QString num = str.mid(1);
        int gpio_num = num.toInt();
        port_num = calc_port_num(gpio_port, gpio_num);

        if(port_num < 0 && str.size() != 0)
        {
            QMessageBox::information(NULL, NULL, tr("Please input true GPIO!"), QMessageBox::Ok);
            qDebug() << "input error GPIO port:" << port_num;
            return;
        }

    }

}

void gpio::language_reload()
{
    ui->retranslateUi(this);
    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::red);
    ui->label_2->setPalette(pe);
    stateGroup->setTitle(tr("state"));
    valueGroup->setTitle(tr("value"));
    ret->setText(tr("return"));
    rBtnout->setText(tr("out"));
    rBtnin->setText(tr("in"));
    rBtnhigh->setText(tr("high"));
    rBtnlow->setText(tr("low"));
    //on_lineedit1_1_editingFinished();
    //on_pushButton_clicked();

}

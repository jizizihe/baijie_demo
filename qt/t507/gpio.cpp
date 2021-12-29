#include "gpio.h"
#include "ui_gpio.h"


gpio::gpio(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gpio)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_StyledBackground,true);

    occupied_gpio = get_debug_gpio();

    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::red);
    ui->label_2->setPalette(pe);
    ui->label_3->setPalette(pe);

    QRegExp regx("[a-zA-Z0-9|,]{1,}$");
    QValidator *validator = new QRegExpValidator(regx, ui->lineedit1_1);
    ui->lineedit1_1->setValidator( validator );

    display = new QTextEdit(this);
    display->setReadOnly(true);
    display->resize(330,320);
    display->move(480,130);
    display->verticalScrollBar()->setStyleSheet("QScrollBar{width:35px;}");

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
    ret->move(10,10);
    ret->setIcon(QIcon(":/t507_button_image/return.png"));

    connect(rBtnin,SIGNAL(clicked(bool)),this,SLOT(rBtnin_clicked()));
    connect(rBtnout,SIGNAL(clicked(bool)),this,SLOT(rBtnout_clicked()));
    connect(rBtnhigh,SIGNAL(clicked(bool)),this,SLOT(rBtnhigh_clicked()));
    connect(rBtnlow,SIGNAL(clicked(bool)),this,SLOT(rBtnlow_clicked()));
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
    for(int i = 0;i < num;i++)
    {
        if(getFileName(port_num[i]))
        {
            gpio_unexport(port_num[i]);
        }
    }
    memset(port_num,0,sizeof(port_num));
    num = 0;
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

    if(!warning())
    {
        return;
    }
    display->clear();

    for(int i = count;i < num;i++)
    {
        gpio_set_state(port_num[i], (char *)"in");

        display->append(QString(tr("gpio_port: %1")).arg(portnum[i]));
        display->append(QString(tr("state: %1")).arg(tr("in")));
        display->append(QString(tr("value: %1")).arg((gpio_get_value(port_num[i]))));
    }
}

void gpio::rBtnout_clicked()
{
    rBtnhigh->setEnabled(true);
    rBtnlow->setEnabled(true);
    rBtnhigh->setCheckable(true);
    rBtnlow->setCheckable(true);

    if(rBtnhigh->isChecked() || rBtnlow->isChecked())
    {
        if(!warning())
        {
            return;
        }
        display->clear();
        for(int i = count;i < num;i++)
        {
            gpio_set_state(port_num[i], (char *)"out");
            portnum_cal(port_num[i],portnum[i]);
            display->append(QString(tr("gpio_port: %1")).arg(portnum[i]));
            display->append(QString(tr("state: %1")).arg(tr("out")));
            if(rBtnhigh->isChecked())
            {
                gpio_set_value(port_num[i], 1);
                display->append(QString(tr("value: %1")).arg(tr("high")));
            }
            else if(rBtnlow->isChecked())
            {
                gpio_set_value(port_num[i], 0);
                display->append(QString(tr("value: %1")).arg(tr("low")));
            }
        }
    }
}

void gpio::rBtnhigh_clicked()
{
    if(rBtnout->isChecked())
    {
        if(!warning())
        {
            return;
        }
        display->clear();
        for(int i = count;i < num;i++)
        {
            gpio_set_state(port_num[i], (char *)"out");
            portnum_cal(port_num[i],portnum[i]);
            display->append(QString(tr("gpio_port: %1")).arg(portnum[i]));
            display->append(QString(tr("state: %1")).arg(tr("out")));
            gpio_set_value(port_num[i], 1);
            display->append(QString(tr("value: %1")).arg(tr("high")));
        }
    }
}

void gpio::rBtnlow_clicked()
{
    if(rBtnout->isChecked())
    {
        if(!warning())
        {
            return;
        }
        display->clear();
        for(int i = count;i < num;i++)
        {
            gpio_set_state(port_num[i], (char *)"out");
            portnum_cal(port_num[i],portnum[i]);
            display->append(QString(tr("gpio_port: %1")).arg(portnum[i]));
            display->append(QString(tr("state: %1")).arg(tr("out")));
            gpio_set_value(port_num[i], 0);
            display->append(QString(tr("value: %1")).arg(tr("low")));
        }
    }
}

bool gpio::warning()
{
    QString str = ui->lineedit1_1->text();
    int i,j = -1;
    QString gpio;

    count = num;

    for(i = 0;i < str.size();i++)
    {
        if(str.at(i) == ',')
        {
            gpio = str.mid(j+1,(i-j-1));
            if(!istrueport(gpio,num))
            {
                QMessageBox::information(NULL, NULL, tr("Please input true GPIO!"), QMessageBox::Ok);
                return false;
            }
            portnum_cal(port_num[num],portnum[num]);
            for(int k = 0;k < occupied_gpio.len;k++)
            {
                if(occupied_gpio.gpio[k] == port_num[num])
                {
                    QMessageBox::information(NULL, NULL, QString(tr("P%1 is occupied!You can't mobilize it!")).arg(portnum[num]), QMessageBox::Ok);
                    return false;
                }
            }
            if(!getFileName(port_num[num]))
            {
                gpio_export(port_num[num]);
            }
            j = i;
            num++;
        }
        else if(i == str.size()-1)
        {
            gpio = str.mid(j+1,(i-j));
            if(!istrueport(gpio,num))
            {
                QMessageBox::information(NULL, NULL, tr("Please input true GPIO!"), QMessageBox::Ok);
                return false;
            }
            portnum_cal(port_num[num],portnum[num]);
            for(int k = 0;k < occupied_gpio.len;k++)
            {
                if(occupied_gpio.gpio[k] == port_num[num])
                {
                    QMessageBox::information(NULL, NULL, QString(tr("P%1 is occupied!You can't mobilize it!")).arg(portnum[num]), QMessageBox::Ok);
                    return false;
                }
            }
            if(!getFileName(port_num[num]))
            {
                gpio_export(port_num[num]);
            }
            j = i;
            num++;
        }
    }
    return true;
}


bool gpio::istrueport(QString str,int i)
{
//    QString str = ui->lineedit1_1->text();
    QString last_character = str.right(1);
    QString third_character = str.mid(2,1);

    if(str.size() == 0)
    {
        return true;
    }

    if(str.size() > 4)
    {
        return false;
    }

    if(str.size() < 2 && str.size() != 0)
    {
        return false;
    }

    if(!isNumber(last_character))
    {
        return false;
    }

    if(isEnglish(third_character) && third_character != "")
    {
        return false;
    }
    //string
    QString port = str.mid(0,1);
    QByteArray temp = port.toLatin1();
    char gpio_port = *(temp.data());
    if(gpio_port == 'p' || gpio_port == 'P')
    {
        QString tmp = str.at(2);
        if(isEnglish(tmp))
        {
            return false;
        }
        port = str.mid(1,2);
        temp = port.toLatin1();
        gpio_port = *(temp.data());

        QString num = str.mid(2);
        int gpio_num = num.toInt();
        port_num[i] = calc_port_num(gpio_port, gpio_num);

        if(port_num[i] < 0 && str.size() != 0)
        {
            return false;
        }

    }
    else
    {
        QString tmp = str.at(1);
        if(isEnglish(tmp))
        {
            return false;
        }
        QString num = str.mid(1);
        int gpio_num = num.toInt();
        port_num[i] = calc_port_num(gpio_port, gpio_num);

        if(port_num[i] < 0 && str.size() != 0)
        {
            return false;
        }
    }
    return true;
}

bool gpio::isEnglish(QString &qstrSrc)
{
    QByteArray ba = qstrSrc.toLatin1();
        const char *s = ba.data();
        bool bret = true;
        while(*s)
        {
            if((*s>='A' && *s<='Z') || (*s>='a' && *s<='z'))
            {

            }
            else
            {
                bret = false;
                break;
            }
            s++;
        }
        return bret;
}

bool gpio::isNumber(QString &qstrSrc)
{
    QByteArray ba = qstrSrc.toLatin1();
    const char *s = ba.data();
    bool bret = true;
    while(*s)
    {
        if(*s>='0' && *s<='9')
        {

        }
        else
        {
            bret = false;
            break;
        }
        s++;
    }
    return bret;
}

void gpio::language_reload()
{
    ui->retranslateUi(this);
    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::red);
    ui->label_2->setPalette(pe);
    ui->label_3->setPalette(pe);
    stateGroup->setTitle(tr("state"));
    valueGroup->setTitle(tr("value"));
//    ret->setText(tr("return"));
    rBtnout->setText(tr("out"));
    rBtnin->setText(tr("in"));
    rBtnhigh->setText(tr("high"));
    rBtnlow->setText(tr("low"));
}



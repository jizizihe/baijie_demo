#include "gpio.h"
#include "ui_gpio.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QScreen>
#include <QDesktopWidget>
#include <QButtonGroup>
#include <QTimer>

static int s_width;
static int s_height;
static int screen_flag;
static QScreen *screen;
static int status_flag;
static int value_flag;
static QLabel *nameLabel1;
static QLabel *nameLabel2;
static QLabel *nameLabel3;
static QLabel *nameLabel4;
static QHBoxLayout *horLayout;
static QTimer *timer;

gpio::gpio(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gpio)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_StyledBackground,true);

    occupied_gpio = get_debug_gpio();

    QRegExp regx("[a-zA-Z0-9|,]{1,}$");
    QValidator *validator = new QRegExpValidator(regx, ui->lineedit1_1);
    ui->lineedit1_1->setValidator( validator );

    ui->display->setReadOnly(true);

    screen = qApp->primaryScreen();
    s_width = screen->size().width();
    s_height = screen->size().height();

    if(s_width < s_height)
    {
        screen_flag = 1;ui->line->setStyleSheet("background-color: rgb(186, 189, 182);");
    }

    ui->status_Switch->setCheckedColor(QColor(100, 225, 100, 150));
    ui->value_Switch->setCheckedColor(QColor(100, 225, 100, 150));

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(gpio_refresh()));
    connect(ui->status_Switch,SIGNAL(toggled(bool)),this,SLOT(BtnChange_flag1(bool)));
    connect(ui->value_Switch,SIGNAL(toggled(bool)),this,SLOT(BtnChange_flag2(bool)));
    settext_statusbtn(1);
    settext_valuebtn(1);
    gpio_font();
    connect(ui->ret,SIGNAL(clicked(bool)),this,SLOT(ret_clicked()));
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
    timer->stop();
}

void gpio::srceenclear()
{
    ui->display->clear();
    ui->lineedit1_1->clear();
//    ui->rBtnout->setCheckable(false);
//    ui->rBtnin->setCheckable(false);
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
                QMessageBox mesg(QMessageBox::Information,
                                 tr("QMessageBox::information()"),
                                 tr("Please input true GPIO!"),
                                 0,this);
                mesg.addButton(tr("OK"),QMessageBox::YesRole);
                if(screen_flag == 1)
                mesg.move(s_width*2/3,s_height/3);
                else
                mesg.move(s_width/3,s_height/3);
                mesg.exec();this->activateWindow();this->setFocus();
                return false;
            }
            portnum_cal(port_num[num],portnum[num]);
            for(int k = 0;k < occupied_gpio.len;k++)
            {
                if(occupied_gpio.gpio[k] == port_num[num])
                {
                    QString str = QString(tr("P%1 is occupied!You can't mobilize it!")).arg(portnum[num]);
                    QMessageBox mesg(QMessageBox::Information,
                                     tr("QMessageBox::information()"),
                                     tr(str.toUtf8()),
                                     0,this);
                    mesg.addButton(tr("OK"),QMessageBox::YesRole);
                    if(screen_flag == 1)
                    mesg.move(s_width*2/3,s_height/3);
                    else
                    mesg.move(s_width/3,s_height/3);
                    mesg.exec();this->activateWindow();this->setFocus();
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
                QString str = QString(tr("Please input true GPIO!")).arg(portnum[num]);
                QMessageBox mesg(QMessageBox::Information,
                                 tr("QMessageBox::information()"),
                                 tr(str.toUtf8()),
                                 0,this);
                mesg.addButton(tr("OK"),QMessageBox::YesRole);
                if(screen_flag == 1)
                mesg.move(s_width*2/3,s_height/3);
                else
                mesg.move(s_width/3,s_height/3);
                mesg.exec();this->activateWindow();this->setFocus();
                return false;
            }
            portnum_cal(port_num[num],portnum[num]);
            for(int k = 0;k < occupied_gpio.len;k++)
            {
                if(occupied_gpio.gpio[k] == port_num[num])
                {
                    QString str = QString(tr("P%1 is occupied!You can't mobilize it!")).arg(portnum[num]);
                    QMessageBox mesg(QMessageBox::Information,
                                     tr("QMessageBox::information()"),
                                     tr(str.toUtf8()),
                                     0,this);
                    mesg.addButton(tr("OK"),QMessageBox::YesRole);
                    if(screen_flag == 1)
                    mesg.move(s_width*2/3,s_height/3);
                    else
                    mesg.move(s_width/3,s_height/3);
                    mesg.exec();this->activateWindow();this->setFocus();
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
    QString str;
    str = nameLabel1->text();
    nameLabel1->setText(tr(str.toUtf8()));
    str = nameLabel2->text();
    nameLabel2->setText(tr(str.toUtf8()));
    str = nameLabel3->text();
    nameLabel3->setText(tr(str.toUtf8()));
    str = nameLabel4->text();
    nameLabel4->setText(tr(str.toUtf8()));
}

void gpio::gpio_font()
{
    qreal realX = screen->physicalDotsPerInchX();
    qreal realY = screen->physicalDotsPerInchY();
    qreal realWidth = s_width / realX * 2.54;
    qreal realHeight = s_height / realY *2.54;
    QFont font;
    if(screen_flag)
    {
        if(realHeight < 15)
        {
            font.setPointSize(12);
        }
        else if (realHeight < 17)
        {
            font.setPointSize(14);
        }
        else
        {
            font.setPointSize(17);
        }

    }
    else
    {
        if(realWidth < 15)
        {
            font.setPointSize(12);
        }
        else if (realWidth < 17)
        {
            font.setPointSize(14);
        }
        else
        {
            font.setPointSize(17);
        }
    }
    ui->label->setFont(font);
    ui->lineedit1_1->setFont(font);
    ui->display->setFont(font);
    //ui->label_2->setFont(font);
    ui->label_3->setFont(font);
    ui->label_4->setFont(font);
}

void gpio::on_pushButton_clicked()
{
   QString str = "If you want to check GPIOG13 ,please input 'g13'.<br>";
   str = str + "You can also operate on multiple GPIO ports, but each two GPIO ports should be separated by ','. ";

    QMessageBox mesg(QMessageBox::Information,
                     tr("QMessageBox::information()"),
                     tr(str.toUtf8()),
                     0,this);
    mesg.addButton(tr("OK"),QMessageBox::YesRole);
    mesg.resize(100,100);
    if(screen_flag == 1)
    mesg.move(s_width*3/4,s_height/4);
    else
    mesg.move(s_width/4,s_height/4);
    mesg.exec();
    this->activateWindow();this->setFocus();
}

void gpio::BtnChange_flag1(bool flag)
{
    flag = ui->status_Switch->isToggled();
    if(flag == 1) // out
    {
        ui->value_Switch->setDisabled(false);
        settext_statusbtn(1);
        if(!warning())
        {
            return;
        }
        ui->display->clear();
        bool flag_v = ui->status_Switch->isToggled();

        for(int i = count;i < num;i++)
        {
            gpio_set_state(port_num[i], (char *)"out");
            portnum_cal(port_num[i],portnum[i]);
            ui->display->setAlignment(Qt::AlignCenter);
            ui->display->append(QString(tr("\n")));
            ui->display->append(QString(tr("  gpio_port: %1")).arg(portnum[i]));
    
            ui->display->append(QString(tr("  state: %1")).arg(tr("out")));
            flag_v = ui->value_Switch->isToggled();
            if(flag_v == 1)
            {
                gpio_set_value(port_num[i], 1);
               // ui->display->append(QString(tr("  value: %1")).arg(tr("high")));
                ui->display->append(QString(tr("  value: 1")));
            }
            else
            {
                gpio_set_value(port_num[i], 0);
               // ui->display->append(QString(tr("  value: %1")).arg(tr("low")));
                ui->display->append(QString(tr("  value: 0")));
            }
        }

    }
    else //in
    {
        settext_statusbtn(0);
        if(!warning())
        {
            return;
        }
        ui->display->clear();    
        for(int i = count;i < num;i++)
        {
            gpio_set_state(port_num[i], (char *)"in");
            ui->display->setAlignment(Qt::AlignCenter);
            ui->display->setAlignment(Qt::AlignCenter);
            ui->display->append(QString(tr("\n")));
            ui->display->append(QString(tr("  gpio_port: %1")).arg(portnum[i]));
            ui->display->append(QString(tr("  state: %1")).arg(tr("in")));
            ui->display->append(QString(tr("  value: %1")).arg((gpio_get_value(port_num[i]))));
        }
        ui->value_Switch->setDisabled(true);
    }    
}

void gpio::BtnChange_flag2(bool flag)
{
    flag = ui->value_Switch->isToggled();
    if(flag == 1) // high
    {
        settext_valuebtn(1);
        if(!warning())
        {
            return;
        }
        ui->display->clear();
        for(int i = count;i < num;i++)
        {
            gpio_set_state(port_num[i], (char *)"out");
            portnum_cal(port_num[i],portnum[i]);
            ui->display->setAlignment(Qt::AlignCenter);
            ui->display->append(QString(tr("\n")));
            ui->display->append(QString(tr("  gpio_port: %1")).arg(portnum[i]));
            
            ui->display->append(QString(tr("  state: %1")).arg(tr("out")));
                gpio_set_value(port_num[i], 1);
                //ui->display->append(QString(tr("  value: %1")).arg(tr("high")));
                ui->display->append(QString(tr("  value: 1")));
        }
    }
    else //close
    {
        settext_valuebtn(0);
        if(!warning())
        {
            return;
        }
        ui->display->clear();
        for(int i = count;i < num;i++)
        {
            gpio_set_state(port_num[i], (char *)"out");
            portnum_cal(port_num[i],portnum[i]);
            ui->display->setAlignment(Qt::AlignCenter);
            ui->display->append(QString(tr("\n")));
            ui->display->append(QString(tr("  gpio_port: %1")).arg(portnum[i]));            
            ui->display->append(QString(tr("  state: %1")).arg(tr("out")));
            gpio_set_value(port_num[i], 0);
           // ui->display->append(QString(tr("  value: %1")).arg(tr("low")));
            ui->display->append(QString(tr("  value: 0")));
        }
    }
}

//void gpio::on_pushButton_2_clicked()
//{
//    if(!warning())
//    {
//        return;
//    }
//    ui->display->clear();
//    bool flag = ui->status_Switch->isToggled();
//    if(flag == 1)
//    {
//        for(int i = count;i < num;i++)
//    {
//        gpio_set_state(port_num[i], (char *)"out");
//        portnum_cal(port_num[i],portnum[i]);
//        ui->display->setA

void gpio::settext_statusbtn(int flag)
{
    if(status_flag == 0)
    {
        nameLabel1 = new QLabel(ui->status_Switch);
        nameLabel2 = new QLabel(ui->status_Switch);
        QHBoxLayout *horLayout = new QHBoxLayout();
        if(flag == 0)
        {
            nameLabel2->setText(tr("direction: in   "));
            horLayout->setStretchFactor(nameLabel1,1);
            horLayout->setStretchFactor(nameLabel2,2);
        }
        else if(flag == 1)
        {
            nameLabel1->setText(tr("   direction: out"));
            horLayout->setStretchFactor(nameLabel1,2);
            horLayout->setStretchFactor(nameLabel2,1);
        }

        nameLabel1->setAlignment(Qt::AlignVCenter);
        nameLabel2->setAlignment(Qt::AlignVCenter);
        nameLabel2->setAlignment(Qt::AlignCenter);
        nameLabel1->setAlignment(Qt::AlignCenter);
        horLayout->addWidget(nameLabel1);
        horLayout->addWidget(nameLabel2);
        ui->status_Switch->setLayout(horLayout);
        int height_switch = ui->status_Switch->height();
        height_switch = height_switch/2;
        horLayout->setContentsMargins(0,0,0,0);
        status_flag++;
    }
    else
    {
        if(flag == 1)
        {
            nameLabel1->setText(tr("  direction: out"));
            nameLabel2->setText("");
            horLayout->setStretchFactor(nameLabel1,2);
            horLayout->setStretchFactor(nameLabel2,1);
        }
        else
        {
            nameLabel1->setText("");
            nameLabel2->setText(tr("direction: in  "));
            horLayout->setStretchFactor(nameLabel1,1);
            horLayout->setStretchFactor(nameLabel2,2);
        }
    }
}

void gpio::settext_valuebtn(int flag)
{
    if(value_flag == 0)
    {
        nameLabel3 = new QLabel(ui->value_Switch);
        nameLabel4 = new QLabel(ui->value_Switch);
        horLayout = new QHBoxLayout();
        if(flag == 0)
        {
            nameLabel4->setText(tr("value: low"));
            //nameLabel3->setText("");
            horLayout->addWidget(nameLabel3);
            horLayout->addWidget(nameLabel4);
            horLayout->setStretchFactor(nameLabel4,2);
            horLayout->setStretchFactor(nameLabel3,1);
        }
        else if(flag == 1)
        {
           // horLayout->setContentsMargins(0,5,0,0);
            horLayout->addWidget(nameLabel3);
            horLayout->addWidget(nameLabel4);
           // nameLabel4->setText("");
            nameLabel3->setText(tr("  value: high"));
            horLayout->setStretchFactor(nameLabel4,1);
            horLayout->setStretchFactor(nameLabel3,2);
        }
        nameLabel3->setAlignment(Qt::AlignVCenter);
        nameLabel4->setAlignment(Qt::AlignVCenter);
        nameLabel4->setAlignment(Qt::AlignCenter);
        nameLabel3->setAlignment(Qt::AlignCenter);
        ui->value_Switch->setLayout(horLayout);
        horLayout->setContentsMargins(0,0,0,0);
        value_flag++;
    }
    else
    {
        if(flag == 0)
        {
            if(screen_flag == 1)
            {
                nameLabel3->setText("");
                nameLabel4->setText(tr("value: low  "));
            }
            else
            {
                nameLabel4->setText(tr("value: low"));
                nameLabel3->setText(" ");
            }
            horLayout->setStretchFactor(nameLabel4,2);
            horLayout->setStretchFactor(nameLabel3,1);
        }
        else
        {
            if(screen_flag == 1)
            {
                nameLabel4->setText("");
                nameLabel3->setText(tr("  value:  high"));
            }
            else
            {
                nameLabel4->setText(" ");
                nameLabel3->setText(tr("  value:  high"));
            }
            horLayout->setStretchFactor(nameLabel4,1);
            horLayout->setStretchFactor(nameLabel3,2);
        }
    }
}

void gpio::showEvent(QShowEvent *event)
{
    timer->start(1000);
}

void gpio::gpio_refresh()
{
    QString str = ui->display->toPlainText();
    if(str.isEmpty())
    {
        return;
    }
    ui->display->clear();
    for(int i = count;i < num;i++)
    {
       // qDebug() << port_num[i];
        QString status_str;
        int status = gpio_get_state(port_num[i]);
        if(status == 1)
        {
            status_str = "out";
        }
        else
        {
            status_str = "in";
        }
        ui->display->setAlignment(Qt::AlignCenter);
        ui->display->append(QString(tr("\n")));
        ui->display->append(QString(tr("  gpio_port: %1")).arg(portnum[i]));
        ui->display->append(QString(tr("  state: %1")).arg(tr(status_str.toUtf8())));
        ui->display->append(QString(tr("  value: %1")).arg(gpio_get_value(port_num[i])));
    }
}

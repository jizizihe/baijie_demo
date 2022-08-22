#include "gpio.h"
#include "ui_gpio.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QScreen>
#include <QDesktopWidget>

static int s_width;
static int s_height;
static int screen_flag;
static QScreen *screen;

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
          screen_flag = 1;
      }
     gpio_font();
     ui->lineedit1_1->setPlaceholderText(tr("please enter gpio port. example: g13"));

    connect(ui->rBtnin,SIGNAL(clicked(bool)),this,SLOT(rBtnin_clicked()));
    connect(ui->rBtnout,SIGNAL(clicked(bool)),this,SLOT(rBtnout_clicked()));
    connect(ui->rBtnhigh,SIGNAL(clicked(bool)),this,SLOT(rBtnhigh_clicked()));
    connect(ui->rBtnlow,SIGNAL(clicked(bool)),this,SLOT(rBtnlow_clicked()));
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

}

void gpio::srceenclear()
{
    ui->display->clear();
    ui->lineedit1_1->clear();
    ui->rBtnout->setCheckable(false);
    ui->rBtnin->setCheckable(false);
    ui->rBtnhigh->setCheckable(false);
    ui->rBtnlow->setCheckable(false);
    ui->rBtnout->setCheckable(true);
    ui->rBtnin->setCheckable(true);
    ui->rBtnhigh->setCheckable(true);
    ui->rBtnlow->setCheckable(true);
    ui->rBtnhigh->setEnabled(true);
    ui->rBtnlow->setEnabled(true);
}

void gpio::rBtnin_clicked()
{
    ui->rBtnhigh->setDisabled(true);
    ui->rBtnlow->setDisabled(true);
    ui->rBtnhigh->setCheckable(false);
    ui->rBtnlow->setCheckable(false);

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
}

void gpio::rBtnout_clicked()
{
    ui->rBtnhigh->setEnabled(true);
    ui->rBtnlow->setEnabled(true);
    ui->rBtnhigh->setCheckable(true);
    ui->rBtnlow->setCheckable(true);

    if(ui->rBtnhigh->isChecked() || ui->rBtnlow->isChecked())
    {
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

            ui->display->append(QString(tr("  gpio_port: %1")).arg(portnum[i]));

            ui->display->append(QString(tr("  state: %1")).arg(tr("out")));
            if(ui->rBtnhigh->isChecked())
            {
                gpio_set_value(port_num[i], 1);
                ui->display->append(QString(tr("  value: %1")).arg(tr("high")));
            }
            else if(ui->rBtnlow->isChecked())
            {
                gpio_set_value(port_num[i], 0);
                ui->display->append(QString(tr("  value: %1")).arg(tr("low")));
            }
        }
    }
}

void gpio::rBtnhigh_clicked()
{
    if(ui->rBtnout->isChecked())
    {
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
            ui-> display->append(QString(tr("  gpio_port: %1")).arg(portnum[i]));
            ui->display->append(QString(tr("  state: %1")).arg(tr("out")));
            gpio_set_value(port_num[i], 1);
            ui->display->append(QString(tr("  value: %1")).arg(tr("high")));
        }
    }
}

void gpio::rBtnlow_clicked()
{
    if(ui->rBtnout->isChecked())
    {
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
            ui->display->append(QString(tr("\n")).arg(portnum[i]));
            ui->display->append(QString(tr("  gpio_port: %1")).arg(portnum[i]));
            ui->display->append(QString(tr("  state: %1")).arg(tr("out")));
            gpio_set_value(port_num[i], 0);
            ui->display->append(QString(tr("  value: %1")).arg(tr("low")));
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
                QMessageBox mesg(QMessageBox::Information,
                                 tr("QMessageBox::information()"),
                                 tr("Please input true GPIO!"),
                                 0,this);
                mesg.addButton(tr("OK"),QMessageBox::YesRole);
                if(screen_flag == 1)
                mesg.move(s_width*2/3,s_height/3);
                else
                mesg.move(s_width/3,s_height/3);
                mesg.exec();
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
                    mesg.exec();
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
                mesg.exec();
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
    ui->stateGroup->setTitle(tr("state"));
    ui->valueGroup->setTitle(tr("value"));
    ui->rBtnout->setText(tr("out"));
    ui->rBtnin->setText(tr("in"));
    ui->rBtnhigh->setText(tr("high"));
    ui->rBtnlow->setText(tr("low"));
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
    ui->rBtnhigh->setFont(font);
    ui->rBtnin->setFont(font);
    ui->rBtnlow->setFont(font);
    ui->rBtnout->setFont(font);
    ui->display->setFont(font);
    ui->stateGroup->setFont(font);
    ui->valueGroup->setFont(font);
}

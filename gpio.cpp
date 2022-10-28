#include "gpio.h"
#include "ui_gpio.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QScreen>
#include <QDesktopWidget>
#include <QButtonGroup>
#include <QTimer>

static QScreen *screen;
static int screenWidth;
static int screenHeight;
static int screenFlag;
static int statusFirstFlag;
static int valueFirstFlag;
static QLabel *statusSwitchLabel1;
static QLabel *statusSwitchLabel2;
static QLabel *valueSwitchLabel1;
static QLabel *valueSwitchLabel2;
static QHBoxLayout *horLayout;
static QTimer *timer;

gpio::gpio(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gpio)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_StyledBackground,true);

    occupiedGpio = getDebugGpio();
    QRegExp regx("[a-zA-Z0-9|,]{1,}$");
    QValidator *validator = new QRegExpValidator(regx, ui->lineedit);
    ui->lineedit->setValidator( validator );
    ui->display->setReadOnly(true);

    screen = qApp->primaryScreen();
    screenWidth = screen->size().width();
    screenHeight = screen->size().height();

    if(screenWidth < screenHeight)
    {
        screenFlag = 1;ui->line->setStyleSheet("background-color: rgb(186, 189, 182);");
    }

    ui->status_Switch->setCheckedColor(QColor(100, 225, 100, 150));
    ui->value_Switch->setCheckedColor(QColor(100, 225, 100, 150));

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(gpio_refresh()));         //The port information is refreshed every second
    connect(ui->status_Switch,SIGNAL(toggled(bool)),this,SLOT(status_switch_change_flag(bool)));
    connect(ui->value_Switch,SIGNAL(toggled(bool)),this,SLOT(value_switch_change_flag(bool)));
    setTextStatusSwitch(1);
    setTextValueSwitch(1);
    gpioFont();
    connect(ui->btn_ret,SIGNAL(clicked(bool)),this,SLOT(ret_clicked()));
    connect(this,SIGNAL(gpio_back_msg()),this,SLOT(srceen_clear()));
}

gpio::~gpio()
{
    delete ui;
    delete statusSwitchLabel1;
    delete statusSwitchLabel2;
    delete valueSwitchLabel1;
    delete valueSwitchLabel2;
    delete horLayout;
}

void gpio::ret_clicked()
{
    emit gpio_back_msg();
    for(int i = 0;i < num;i++)
    {
        if(getFileName(portNumInt[i]))
        {
            gpioUnexport(portNumInt[i]);
        }
    }
    memset(portNumInt,0,sizeof(portNumInt));
    num = 0;
    timer->stop();
}

void gpio::srceen_clear()
{
    ui->display->clear();
    ui->lineedit->clear();
}

bool gpio::warning()
{
    QString strPort = ui->lineedit->text();
    int i,j = -1;
    QString gpio;
    portCount = num;

    for(i = 0;i < strPort.size();i++)
    {
        if(strPort.at(i) == ',')
        {
            gpio = strPort.mid(j+1,(i-j-1));
            if(!isTruePort(gpio,num))
            {
                QMessageBox mesg(QMessageBox::Information,
                                 tr("QMessageBox::information()"),
                                 tr("Please input true GPIO!"),
                                 0,this);
                mesg.addButton(tr("OK"),QMessageBox::YesRole);
                if(screenFlag == 1)
                    mesg.move(screenWidth*2/3,screenHeight/3);
                else
                    mesg.move(screenWidth/3,screenHeight/3);
                mesg.exec();this->activateWindow();this->setFocus();
                return false;
            }
            calcPortStr(portNumInt[num],portNumStr[num]);
            for(int k = 0;k < occupiedGpio.len;k++)
            {
                if(occupiedGpio.gpio[k] == portNumInt[num])
                {
                    QString str = QString(tr("P%1 is occupied!You can't mobilize it!")).arg(portNumStr[num]);
                    QMessageBox mesg(QMessageBox::Information,
                                     tr("QMessageBox::information()"),
                                     tr(str.toUtf8()),
                                     0,this);
                    mesg.addButton(tr("OK"),QMessageBox::YesRole);
                    if(screenFlag == 1)
                        mesg.move(screenWidth*2/3,screenHeight/3);
                    else
                        mesg.move(screenWidth/3,screenHeight/3);
                    mesg.exec();this->activateWindow();this->setFocus();
                    return false;
                }
            }
            if(!getFileName(portNumInt[num]))
            {
                gpioExport(portNumInt[num]);
            }
            j = i;
            num++;
        }
        else if(i == strPort.size()-1)
        {
            gpio = strPort.mid(j+1,(i-j));
            if(!isTruePort(gpio,num))
            {
                QString str = QString(tr("Please input true GPIO!"));
                QMessageBox mesg(QMessageBox::Information,
                                 tr("QMessageBox::information()"),
                                 tr(str.toUtf8()),
                                 0,this);
                mesg.addButton(tr("OK"),QMessageBox::YesRole);
                if(screenFlag == 1)
                    mesg.move(screenWidth*2/3,screenHeight/3);
                else
                    mesg.move(screenWidth/3,screenHeight/3);
                mesg.exec();this->activateWindow();this->setFocus();
                return false;
            }
            calcPortStr(portNumInt[num],portNumStr[num]);
            for(int k = 0;k < occupiedGpio.len;k++)
            {
                if(occupiedGpio.gpio[k] == portNumInt[num])
                {
                    QString str = QString(tr("P%1 is occupied!You can't mobilize it!")).arg(portNumStr[num]);
                    QMessageBox mesg(QMessageBox::Information,
                                     tr("QMessageBox::information()"),
                                     tr(str.toUtf8()),
                                     0,this);
                    mesg.addButton(tr("OK"),QMessageBox::YesRole);
                    if(screenFlag == 1)
                        mesg.move(screenWidth*2/3,screenHeight/3);
                    else
                        mesg.move(screenWidth/3,screenHeight/3);
                    mesg.exec();this->activateWindow();this->setFocus();
                    return false;
                }
            }
            if(!getFileName(portNumInt[num]))
            {
                gpioExport(portNumInt[num]);
            }
            j = i;
            num++;
        }
    }
    return true;
}

bool gpio::isTruePort(QString str,int i)             //Check whether the port mode is correct
{
    QString lastCharacter = str.right(1);
    QString thirdCharacter = str.mid(2,1);

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

    if(!isNumber(lastCharacter))
    {
        return false;
    }

    if(isEnglish(thirdCharacter) && thirdCharacter != "")
    {
        return false;
    }
    QString port = str.mid(0,1);
    QByteArray temp = port.toLatin1();
    char gpioPort = *(temp.data());
    if(gpioPort == 'p' || gpioPort == 'P')
    {
        QString tmp = str.at(2);
        if(isEnglish(tmp))
        {
            return false;
        }
        port = str.mid(1,2);
        temp = port.toLatin1();
        gpioPort = *(temp.data());

        QString num = str.mid(2);
        int gpio_num = num.toInt();
        portNumInt[i] = calcPortNum(gpioPort, gpio_num);

        if(portNumInt[i] < 0 && str.size() != 0)
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
        int gpioNum = num.toInt();
        portNumInt[i] = calcPortNum(gpioPort, gpioNum);

        if(portNumInt[i] < 0 && str.size() != 0)
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

void gpio::languageReload()
{
    ui->retranslateUi(this);
    QString str;
    str = statusSwitchLabel1->text();
    statusSwitchLabel1->setText(tr(str.toUtf8()));
    str = statusSwitchLabel2->text();
    statusSwitchLabel2->setText(tr(str.toUtf8()));
    str = valueSwitchLabel1->text();
    valueSwitchLabel1->setText(tr(str.toUtf8()));
    str = valueSwitchLabel2->text();
    valueSwitchLabel2->setText(tr(str.toUtf8()));
}

void gpio::gpioFont()
{
    qreal realX = screen->physicalDotsPerInchX();
    qreal realY = screen->physicalDotsPerInchY();
    qreal realWidth = screenWidth / realX * 2.54;
    qreal realHeight = screenHeight / realY *2.54;
    QFont font;
    if(screenFlag)
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
    ui->lbl_gpio->setFont(font);
    ui->lineedit->setFont(font);
    ui->display->setFont(font);
    ui->lbl_gpioTitle->setFont(font);
    ui->lbl_gpioStatus->setFont(font);
}

void gpio::on_btn_hint_clicked()
{
    QString str = "If you want to check GPIOG13 ,please input 'g13'.<br>";
    str = str + "You can also operate on multiple GPIO ports, but each two GPIO ports should be separated by ','. ";

    QMessageBox mesg(QMessageBox::Information,
                     tr("QMessageBox::information()"),
                     tr(str.toUtf8()),
                     0,this);
    mesg.addButton(tr("OK"),QMessageBox::YesRole);
    mesg.resize(100,100);
    if(screenFlag == 1)
        mesg.move(screenWidth*3/4,screenHeight/4);
    else
        mesg.move(screenWidth/4,screenHeight/4);
    mesg.exec();
    this->activateWindow();this->setFocus();
}

void gpio::status_switch_change_flag(bool flag)
{
    flag = ui->status_Switch->isToggled();
    if(flag == 1) // out
    {
        ui->value_Switch->setDisabled(false);
        setTextStatusSwitch(1);
        if(!warning())
        {
            return;
        }
        ui->display->clear();
        for(int i = portCount;i < num;i++)
        {
            gpioSetState(portNumInt[i], (char *)"out");
            calcPortStr(portNumInt[i],portNumStr[i]);
            ui->display->setAlignment(Qt::AlignCenter);
            ui->display->append(QString(tr("\n")));
            ui->display->append(QString(tr("  gpio_port: %1")).arg(portNumStr[i]));

            ui->display->append(QString(tr("  state: %1")).arg(tr("out")));
            bool valueFlag = ui->value_Switch->isToggled();
            if(valueFlag == 1)
            {
                gpioSetValue(portNumInt[i], 1);
                ui->display->append(QString(tr("  value: 1")));
            }
            else
            {
                gpioSetValue(portNumInt[i], 0);
                ui->display->append(QString(tr("  value: 0")));
            }
        }

    }
    else //in
    {
        setTextStatusSwitch(0);
        if(!warning())
        {
            return;
        }
        ui->display->clear();
        for(int i = portCount;i < num;i++)
        {
            gpioSetState(portNumInt[i], (char *)"in");
            ui->display->setAlignment(Qt::AlignCenter);
            ui->display->setAlignment(Qt::AlignCenter);
            ui->display->append(QString(tr("\n")));
            ui->display->append(QString(tr("  gpio_port: %1")).arg(portNumStr[i]));
            ui->display->append(QString(tr("  state: %1")).arg(tr("in")));
            ui->display->append(QString(tr("  value: %1")).arg((gpioGetValue(portNumInt[i]))));
        }
        ui->value_Switch->setDisabled(true);
    }
}

void gpio::value_switch_change_flag(bool flag)
{
    flag = ui->value_Switch->isToggled();
    if(flag == 1) // high
    {
        setTextValueSwitch(1);
        if(!warning())
        {
            return;
        }
        ui->display->clear();
        for(int i = portCount;i < num;i++)
        {
            gpioSetState(portNumInt[i], (char *)"out");
            calcPortStr(portNumInt[i],portNumStr[i]);
            ui->display->setAlignment(Qt::AlignCenter);
            ui->display->append(QString(tr("\n")));
            ui->display->append(QString(tr("  gpio_port: %1")).arg(portNumStr[i]));
            ui->display->append(QString(tr("  state: %1")).arg(tr("out")));
            gpioSetValue(portNumInt[i], 1);
            ui->display->append(QString(tr("  value: 1")));
        }
    }
    else //low
    {
        setTextValueSwitch(0);
        if(!warning())
        {
            return;
        }
        ui->display->clear();
        for(int i = portCount;i < num;i++)
        {
            gpioSetState(portNumInt[i], (char *)"out");
            calcPortStr(portNumInt[i],portNumStr[i]);
            ui->display->setAlignment(Qt::AlignCenter);
            ui->display->append(QString(tr("\n")));
            ui->display->append(QString(tr("  gpio_port: %1")).arg(portNumStr[i]));
            ui->display->append(QString(tr("  state: %1")).arg(tr("out")));
            gpioSetValue(portNumInt[i], 0);
            ui->display->append(QString(tr("  value: 0")));
        }
    }
}

void gpio::setTextStatusSwitch(int flag)
{
    if(statusFirstFlag == 0)
    {
        statusSwitchLabel1 = new QLabel(ui->status_Switch);
        statusSwitchLabel2 = new QLabel(ui->status_Switch);
        QHBoxLayout *horLayout = new QHBoxLayout();
        if(flag == 0)
        {
            statusSwitchLabel2->setText(tr("direction: in   "));
            horLayout->setStretchFactor(statusSwitchLabel1,1);
            horLayout->setStretchFactor(statusSwitchLabel2,2);
        }
        else if(flag == 1)
        {
            statusSwitchLabel1->setText(tr("   direction: out"));
            horLayout->setStretchFactor(statusSwitchLabel1,2);
            horLayout->setStretchFactor(statusSwitchLabel2,1);
        }

        statusSwitchLabel1->setAlignment(Qt::AlignVCenter);
        statusSwitchLabel2->setAlignment(Qt::AlignVCenter);
        statusSwitchLabel2->setAlignment(Qt::AlignCenter);
        statusSwitchLabel1->setAlignment(Qt::AlignCenter);
        horLayout->addWidget(statusSwitchLabel1);
        horLayout->addWidget(statusSwitchLabel2);
        ui->status_Switch->setLayout(horLayout);
        horLayout->setContentsMargins(0,0,0,0);
        statusFirstFlag++;
    }
    else
    {
        if(flag == 1)
        {
            statusSwitchLabel1->setText(tr("  direction: out"));
            statusSwitchLabel2->setText("");
            horLayout->setStretchFactor(statusSwitchLabel1,2);
            horLayout->setStretchFactor(statusSwitchLabel2,1);
        }
        else
        {
            statusSwitchLabel1->setText("");
            statusSwitchLabel2->setText(tr("direction: in  "));
            horLayout->setStretchFactor(statusSwitchLabel1,1);
            horLayout->setStretchFactor(statusSwitchLabel2,2);
        }
    }
}

void gpio::setTextValueSwitch(int flag)
{
    if(valueFirstFlag == 0)
    {
        valueSwitchLabel1 = new QLabel(ui->value_Switch);
        valueSwitchLabel2 = new QLabel(ui->value_Switch);
        horLayout = new QHBoxLayout();
        if(flag == 0)
        {
            valueSwitchLabel2->setText(tr("value: low"));
            horLayout->addWidget(valueSwitchLabel1);
            horLayout->addWidget(valueSwitchLabel2);
            horLayout->setStretchFactor(valueSwitchLabel2,2);
            horLayout->setStretchFactor(valueSwitchLabel1,1);
        }
        else if(flag == 1)
        {
            horLayout->addWidget(valueSwitchLabel1);
            horLayout->addWidget(valueSwitchLabel2);
            valueSwitchLabel1->setText(tr("  value: high"));
            horLayout->setStretchFactor(valueSwitchLabel2,1);
            horLayout->setStretchFactor(valueSwitchLabel1,2);
        }
        valueSwitchLabel1->setAlignment(Qt::AlignVCenter);
        valueSwitchLabel2->setAlignment(Qt::AlignVCenter);
        valueSwitchLabel2->setAlignment(Qt::AlignCenter);
        valueSwitchLabel1->setAlignment(Qt::AlignCenter);
        ui->value_Switch->setLayout(horLayout);
        horLayout->setContentsMargins(0,0,0,0);
        valueFirstFlag++;
    }
    else
    {
        if(flag == 0)
        {
            if(screenFlag == 1)
            {
                valueSwitchLabel1->setText("");
                valueSwitchLabel2->setText(tr("value: low  "));
            }
            else
            {
                valueSwitchLabel2->setText(tr("value: low"));
                valueSwitchLabel1->setText(" ");
            }
            horLayout->setStretchFactor(valueSwitchLabel2,2);
            horLayout->setStretchFactor(valueSwitchLabel1,1);
        }
        else
        {
            if(screenFlag == 1)
            {
                valueSwitchLabel2->setText("");
                valueSwitchLabel1->setText(tr("  value:  high"));
            }
            else
            {
                valueSwitchLabel2->setText(" ");
                valueSwitchLabel1->setText(tr("  value:  high"));
            }
            horLayout->setStretchFactor(valueSwitchLabel2,1);
            horLayout->setStretchFactor(valueSwitchLabel1,2);
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
    for(int i = portCount;i < num;i++)
    {
        QString strStatus;
        int status = gpioGetState(portNumInt[i]);
        if(status == 1)
        {
            strStatus = "out";
        }
        else
        {
            strStatus = "in";
        }
        ui->display->setAlignment(Qt::AlignCenter);
        ui->display->append(QString(tr("\n")));
        ui->display->append(QString(tr("  gpio_port: %1")).arg(portNumStr[i]));
        ui->display->append(QString(tr("  state: %1")).arg(tr(strStatus.toUtf8())));
        ui->display->append(QString(tr("  value: %1")).arg(gpioGetValue(portNumInt[i])));
    }
}

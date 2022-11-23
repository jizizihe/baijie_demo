#include "gpio.h"
#include "ui_gpio.h"
#include <QScreen>
#include <QButtonGroup>
#include <QTimer>

static QScreen *g_screen;
static int g_screenWidth;
static int g_screenHeight;
static int g_statusFirstFlag;        // 0:Display status button for the first time
static int g_valueFirstFlag;         // 0:Display value button for the first time
static QLabel *g_statusSwitchLabel1;
static QLabel *g_statusSwitchLabel2;
static QLabel *g_valueSwitchLabel1;
static QLabel *g_valueSwitchLabel2;
static QHBoxLayout *g_valueHorLayout;
static QHBoxLayout *g_statusHorLayout;
static QTimer *g_timer;

gpio::gpio(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gpio)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground,true);

    g_occupiedGpio = getDebugGpio();
    QRegExp regx("[a-zA-Z0-9|,]{1,}$");
    QValidator *validator = new QRegExpValidator(regx, ui->lineedit);
    ui->lineedit->setValidator( validator );
    ui->display->setReadOnly(true);

    g_screen = qApp->primaryScreen();
    g_screenWidth = g_screen->size().width();
    g_screenHeight = g_screen->size().height();

    ui->status_Switch->setCheckedColor(QColor(100, 225, 100, 150));
    ui->value_Switch->setCheckedColor(QColor(100, 225, 100, 150));

    g_timer = new QTimer(this);
    connect(g_timer,SIGNAL(timeout()),this,SLOT(gpio_refresh()));         // The port information is refreshed every second
    connect(ui->status_Switch,SIGNAL(toggled(bool)),this,SLOT(status_switch_change_flag(bool)));
    connect(ui->value_Switch,SIGNAL(toggled(bool)),this,SLOT(value_switch_change_flag(bool)));
    connect(ui->btn_ret,SIGNAL(clicked(bool)),this,SLOT(btn_ret_clicked()));
    connect(this,SIGNAL(gpio_back_msg()),this,SLOT(srceen_clear()));

    setTextStatusSwitch(1);
    setTextValueSwitch(1);
    setGpioFont();
}

gpio::~gpio()
{
    delete ui;
    delete g_statusSwitchLabel1;
    delete g_statusSwitchLabel2;
    delete g_valueSwitchLabel1;
    delete g_valueSwitchLabel2;
    delete g_valueHorLayout;
    delete g_statusHorLayout;
}

void gpio::btn_ret_clicked()
{
    emit gpio_back_msg();
    for(int i = 0;i < g_portNum;i++)
    {
        if(getFileName(g_portNumInt[i]))
        {
            gpioUnexport(g_portNumInt[i]);
        }
    }
    memset(g_portNumInt,0,sizeof(g_portNumInt));
    g_portNum = 0;
    g_timer->stop();
}

void gpio::srceen_clear()
{
    ui->display->clear();
    ui->lineedit->clear();
}

bool gpio::input_warning()
{
    QString strPort = ui->lineedit->text();
    int i,j = -1;
    QString gpio;
    g_portCount = g_portNum;

    for(i = 0;i < strPort.size();i++)
    {
        if(strPort.at(i) == ',')
        {
            gpio = strPort.mid(j+1,(i-j-1));
            if(!isTruePort(gpio,g_portNum))
            {
                QMessageBox mesg(QMessageBox::Information,
                                 tr("QMessageBox::information()"),
                                 tr("Please input true GPIO!"),
                                 0,this);
                mesg.addButton(tr("OK"),QMessageBox::YesRole);
                mesg.move(g_screenWidth/3,g_screenHeight/3);
                mesg.exec();this->activateWindow();this->setFocus();
                return false;
            }
            calcPortStr(g_portNumInt[g_portNum],g_portNumStr[g_portNum]);
            for(int k = 0;k < g_occupiedGpio.bufLength;k++)
            {
                if(g_occupiedGpio.gpio[k] == g_portNumInt[g_portNum])
                {
                    QString str = QString(tr("P%1 is occupied!You can't mobilize it!")).arg(g_portNumStr[g_portNum]);
                    QMessageBox mesg(QMessageBox::Information,
                                     tr("QMessageBox::information()"),
                                     tr(str.toUtf8()),
                                     0,this);
                    mesg.addButton(tr("OK"),QMessageBox::YesRole);
                    mesg.move(g_screenWidth/3,g_screenHeight/3);
                    mesg.exec();this->activateWindow();this->setFocus();
                    return false;
                }
            }
            if(!getFileName(g_portNumInt[g_portNum]))
            {
                gpioExport(g_portNumInt[g_portNum]);
            }
            j = i;
            g_portNum++;
        }
        else if(i == strPort.size()-1)
        {
            gpio = strPort.mid(j+1,(i-j));
            if(!isTruePort(gpio,g_portNum))
            {
                QString str = QString(tr("Please input true GPIO!"));
                QMessageBox mesg(QMessageBox::Information,
                                 tr("QMessageBox::information()"),
                                 tr(str.toUtf8()),
                                 0,this);
                mesg.addButton(tr("OK"),QMessageBox::YesRole);
                mesg.move(g_screenWidth/3,g_screenHeight/3);
                mesg.exec();this->activateWindow();this->setFocus();
                return false;
            }
            calcPortStr(g_portNumInt[g_portNum],g_portNumStr[g_portNum]);
            for(int k = 0;k < g_occupiedGpio.bufLength;k++)
            {
                if(g_occupiedGpio.gpio[k] == g_portNumInt[g_portNum])
                {
                    QString str = QString(tr("P%1 is occupied!You can't mobilize it!")).arg(g_portNumStr[g_portNum]);
                    QMessageBox mesg(QMessageBox::Information,
                                     tr("QMessageBox::information()"),
                                     tr(str.toUtf8()),
                                     0,this);
                    mesg.addButton(tr("OK"),QMessageBox::YesRole);
                    mesg.move(g_screenWidth/3,g_screenHeight/3);
                    mesg.exec();this->activateWindow();this->setFocus();
                    return false;
                }
            }
            if(!getFileName(g_portNumInt[g_portNum]))
            {
                gpioExport(g_portNumInt[g_portNum]);
            }
            j = i;
            g_portNum++;
        }
    }
    return true;
}

bool gpio::isTruePort(QString str,int i)             // Check whether the port mode is correct
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

        QString g_portNum = str.mid(2);
        int gpio_num = g_portNum.toInt();
        g_portNumInt[i] = calcPortNum(gpioPort, gpio_num);

        if(g_portNumInt[i] < 0 && str.size() != 0)
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
        QString g_portNum = str.mid(1);
        int gpioNum = g_portNum.toInt();
        g_portNumInt[i] = calcPortNum(gpioPort, gpioNum);

        if(g_portNumInt[i] < 0 && str.size() != 0)
        {
            return false;
        }
    }
    return true;
}

bool gpio::isEnglish(QString &qstrSrc)
{
    QByteArray ba = qstrSrc.toLatin1();
    const char *str = ba.data();
    bool bret = true;
    while(*str)
    {
        if((*str>='A' && *str<='Z') || (*str>='a' && *str<='z'))
        {

        }
        else
        {
            bret = false;
            break;
        }
        str++;
    }
    return bret;
}

bool gpio::isNumber(QString &qstrSrc)
{
    QByteArray ba = qstrSrc.toLatin1();
    const char *str = ba.data();
    bool bret = true;
    while(*str)
    {
        if(*str>='0' && *str<='9')
        {

        }
        else
        {
            bret = false;
            break;
        }
        str++;
    }
    return bret;
}

void gpio::languageReload()
{
    ui->retranslateUi(this);
    int flag = ui->status_Switch->isToggled();
    if(flag == 1)
    ui->status_Switch->setToggle(true);
    else
    ui->status_Switch->setToggle(false);

    flag = ui->value_Switch->isToggled();
    if(flag == 1)
    ui->value_Switch->setToggle(true);
    else
    ui->value_Switch->setToggle(false);
}

void gpio::setGpioFont()
{
    qreal realX = g_screen->physicalDotsPerInchX();
    qreal realWidth = g_screenWidth / realX * 2.54;
    QFont font;
    if(realWidth < 15)
    {
        font.setPointSize(12);
    }
    else if (realWidth < 18)
    {
        font.setPointSize(14);
    }
    else
    {
        font.setPointSize(17);
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
    mesg.resize(g_screenWidth/3,g_screenHeight/2);
    mesg.move(g_screenWidth/3,g_screenHeight/2-g_screenHeight/4);
    mesg.exec();
}

void gpio::status_switch_change_flag(bool flag)
{
    flag = ui->status_Switch->isToggled();
    if(flag == 1) // Out
    {
        ui->value_Switch->setDisabled(false);
        setTextStatusSwitch(1);
        if(!input_warning())
        {
            return;
        }
        ui->display->clear();
        for(int i = g_portCount;i < g_portNum;i++)
        {
            setGpioState(g_portNumInt[i], (char *)"out");
            calcPortStr(g_portNumInt[i],g_portNumStr[i]);
            ui->display->setAlignment(Qt::AlignCenter);
            ui->display->append(QString(tr("\n")));
            ui->display->append(QString(tr("  gpio_port: %1")).arg(g_portNumStr[i]));

            ui->display->append(QString(tr("  state: %1")).arg(tr("out")));
            bool valueFlag = ui->value_Switch->isToggled();
            if(valueFlag == 1)
            {
                setGpioValue(g_portNumInt[i], 1);
                ui->display->append(QString(tr("  value: 1")));
            }
            else
            {
                setGpioValue(g_portNumInt[i], 0);
                ui->display->append(QString(tr("  value: 0")));
            }
        }

    }
    else // In
    {
        setTextStatusSwitch(0);
        if(!input_warning())
        {
            return;
        }
        ui->display->clear();
        for(int i = g_portCount;i < g_portNum;i++)
        {
            setGpioState(g_portNumInt[i], (char *)"in");
            ui->display->setAlignment(Qt::AlignCenter);
            ui->display->setAlignment(Qt::AlignCenter);
            ui->display->append(QString(tr("\n")));
            ui->display->append(QString(tr("  gpio_port: %1")).arg(g_portNumStr[i]));
            ui->display->append(QString(tr("  state: %1")).arg(tr("in")));
            ui->display->append(QString(tr("  value: %1")).arg((getGpioValue(g_portNumInt[i]))));
        }
        ui->value_Switch->setDisabled(true);
    }
}

void gpio::value_switch_change_flag(bool flag)
{
    flag = ui->value_Switch->isToggled();
    if(flag == 1) // High
    {
        setTextValueSwitch(1);
        if(!input_warning())
        {
            return;
        }
        ui->display->clear();
        for(int i = g_portCount;i < g_portNum;i++)
        {
            setGpioState(g_portNumInt[i], (char *)"out");
            calcPortStr(g_portNumInt[i],g_portNumStr[i]);
            ui->display->setAlignment(Qt::AlignCenter);
            ui->display->append(QString(tr("\n")));
            ui->display->append(QString(tr("  gpio_port: %1")).arg(g_portNumStr[i]));
            ui->display->append(QString(tr("  state: %1")).arg(tr("out")));
            setGpioValue(g_portNumInt[i], 1);
            ui->display->append(QString(tr("  value: 1")));
        }
    }
    else // Low
    {
        setTextValueSwitch(0);
        if(!input_warning())
        {
            return;
        }
        ui->display->clear();
        for(int i = g_portCount;i < g_portNum;i++)
        {
            setGpioState(g_portNumInt[i], (char *)"out");
            calcPortStr(g_portNumInt[i],g_portNumStr[i]);
            ui->display->setAlignment(Qt::AlignCenter);
            ui->display->append(QString(tr("\n")));
            ui->display->append(QString(tr("  gpio_port: %1")).arg(g_portNumStr[i]));
            ui->display->append(QString(tr("  state: %1")).arg(tr("out")));
            setGpioValue(g_portNumInt[i], 0);
            ui->display->append(QString(tr("  value: 0")));
        }
    }
}

void gpio::setTextStatusSwitch(int flag)
{
    if(g_statusFirstFlag == 0)
    {
        g_statusSwitchLabel1 = new QLabel(ui->status_Switch);
        g_statusSwitchLabel2 = new QLabel(ui->status_Switch);
        g_statusHorLayout = new QHBoxLayout();
        if(flag == 0)
        {
            g_statusSwitchLabel2->setText(tr("direction: in   "));
            g_statusHorLayout->setStretchFactor(g_statusSwitchLabel1,1);
            g_statusHorLayout->setStretchFactor(g_statusSwitchLabel2,2);
        }
        else if(flag == 1)
        {
            g_statusSwitchLabel1->setText(tr("   direction: out"));
            g_statusHorLayout->setStretchFactor(g_statusSwitchLabel1,2);
            g_statusHorLayout->setStretchFactor(g_statusSwitchLabel2,1);
        }

        g_statusSwitchLabel1->setAlignment(Qt::AlignVCenter);
        g_statusSwitchLabel2->setAlignment(Qt::AlignVCenter);
        g_statusSwitchLabel2->setAlignment(Qt::AlignCenter);
        g_statusSwitchLabel1->setAlignment(Qt::AlignCenter);
        g_statusHorLayout->addWidget(g_statusSwitchLabel1);
        g_statusHorLayout->addWidget(g_statusSwitchLabel2);
        ui->status_Switch->setLayout(g_statusHorLayout);
        g_statusHorLayout->setContentsMargins(0,0,0,0);
        g_statusFirstFlag++;
    }
    else
    {
        if(flag == 1)
        {
            g_statusSwitchLabel1->setText(tr("  direction: out"));
            g_statusSwitchLabel2->setText("");
            g_statusHorLayout->setStretchFactor(g_statusSwitchLabel1,2);
            g_statusHorLayout->setStretchFactor(g_statusSwitchLabel2,1);
        }
        else
        {
            g_statusSwitchLabel1->setText("");
            g_statusSwitchLabel2->setText(tr("direction: in  "));
            g_statusHorLayout->setStretchFactor(g_statusSwitchLabel1,1);
            g_statusHorLayout->setStretchFactor(g_statusSwitchLabel2,2);
        }
    }
}

void gpio::setTextValueSwitch(int flag)
{
    if(g_valueFirstFlag == 0)
    {
        g_valueSwitchLabel1 = new QLabel(ui->value_Switch);
        g_valueSwitchLabel2 = new QLabel(ui->value_Switch);
        g_valueHorLayout = new QHBoxLayout();
        if(flag == 0)
        {
            g_valueSwitchLabel2->setText(tr("value: low"));
            g_valueHorLayout->addWidget(g_valueSwitchLabel1);
            g_valueHorLayout->addWidget(g_valueSwitchLabel2);
            g_valueHorLayout->setStretchFactor(g_valueSwitchLabel2,2);
            g_valueHorLayout->setStretchFactor(g_valueSwitchLabel1,1);
        }
        else if(flag == 1)
        {
            g_valueHorLayout->addWidget(g_valueSwitchLabel1);
            g_valueHorLayout->addWidget(g_valueSwitchLabel2);
            g_valueSwitchLabel1->setText(tr("  value: high"));
            g_valueHorLayout->setStretchFactor(g_valueSwitchLabel2,1);
            g_valueHorLayout->setStretchFactor(g_valueSwitchLabel1,2);
        }
        g_valueSwitchLabel1->setAlignment(Qt::AlignVCenter);
        g_valueSwitchLabel2->setAlignment(Qt::AlignVCenter);
        g_valueSwitchLabel2->setAlignment(Qt::AlignCenter);
        g_valueSwitchLabel1->setAlignment(Qt::AlignCenter);
        ui->value_Switch->setLayout(g_valueHorLayout);
        g_valueHorLayout->setContentsMargins(0,0,0,0);
        g_valueFirstFlag++;
    }
    else
    {
        if(flag == 0)
        {
            g_valueSwitchLabel2->setText(tr("value: low  "));
            g_valueSwitchLabel1->setText(" ");
            g_valueHorLayout->setStretchFactor(g_valueSwitchLabel2,2);
            g_valueHorLayout->setStretchFactor(g_valueSwitchLabel1,1);
        }
        else
        {
            g_valueSwitchLabel2->setText(" ");
            g_valueSwitchLabel1->setText(tr("  value:  high"));
            g_valueHorLayout->setStretchFactor(g_valueSwitchLabel2,1);
            g_valueHorLayout->setStretchFactor(g_valueSwitchLabel1,2);
        }
    }
}

void gpio::showEvent(QShowEvent *event)
{
    g_timer->start(1000);
    QWidget::showEvent(event);
}

void gpio::gpio_refresh()
{
    QString str = ui->display->toPlainText();
    if(str.isEmpty())
    {
        return;
    }
    ui->display->clear();
    for(int i = g_portCount;i < g_portNum;i++)
    {
        QString strStatus;
        int status = getGpioState(g_portNumInt[i]);
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
        ui->display->append(QString(tr("  gpio_port: %1")).arg(g_portNumStr[i]));
        ui->display->append(QString(tr("  state: %1")).arg(tr(strStatus.toUtf8())));
        ui->display->append(QString(tr("  value: %1")).arg(getGpioValue(g_portNumInt[i])));
    }
}

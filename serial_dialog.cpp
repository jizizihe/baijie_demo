#include "serial_dialog.h"
#include "ui_serialdialog.h"
#include <QScreen>

static int g_screenWidth;
static int g_screenHeight;
static int g_screenFlag;
static QScreen *g_screen;

serialdialog::serialdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::serialdialog)
{
    ui->setupUi(this);
    g_screen = qApp->primaryScreen();
    g_screenWidth = g_screen->size().width();
    g_screenHeight = g_screen->size().height();

    if(g_screenWidth < g_screenHeight)
    {
        g_screenFlag = 1;
    }
    QFont font;
    qreal realX = g_screen->physicalDotsPerInchX();
    qreal realY = g_screen->physicalDotsPerInchY();
    qreal realWidth = g_screenWidth / realX * 2.54;
    qreal realHeight = g_screenHeight / realY *2.54;

    if(g_screenFlag == 1)
    {
        if(realHeight < 15)
        {
            font.setPointSize(10);
        }
        else if (realHeight < 17)
        {
            font.setPointSize(12);
        }
        else
        {
            font.setPointSize(14);
        }

    }
    else
    {
        if(realWidth < 15)
        {
            font.setPointSize(10);
        }
        else if (realWidth < 17)
        {
            font.setPointSize(12);
        }
        else
        {
            font.setPointSize(14);
        }
    }
    ui->lbl_serialSet->setFont(font);
    ui->lbl_modeChoose->setFont(font);
    ui->groupBox->setFont(font);
    ui->serialModeBox->setFont(font);
    ui->btn_serialCancel->setFont(font);
    ui->btn_serialCheckAll->setFont(font);
    ui->btn_serialOk->setFont(font);

    g_pButtonGroup = new QButtonGroup(this);
    g_pButtonGroup->setExclusive(false);               //Set to non-exclusive mode

    QStringList serialPortName;
    QString tmp;
    QGridLayout  *vLayout1 = new QGridLayout ;

    //Find available serial ports
    foreach(const QSerialPortInfo &Info,QSerialPortInfo::availablePorts())//Read serial port information
    {
        if(Info.portName() == QString("ttyS0"))
        {
            continue;
        }
        serialPortName << Info.portName();
    }

    for(int i = 0;i < serialPortName.size(); i++)
    {
        tmp = serialPortName.at(i);
        QCheckBox *checkbox = new QCheckBox;
        checkbox->setText(QString(tmp));
        checkbox->setFont(font);
        vLayout1->addWidget(checkbox,i/2,i%2);
        g_pButtonGroup->addButton(checkbox,i);
    }

    ui->groupBox->setLayout(vLayout1);
    connect(g_pButtonGroup,SIGNAL(buttonClicked(int)),this,SLOT(btn_group_pressed_func(int)));
}

serialdialog::~serialdialog()
{
    delete ui;
}

void serialdialog::getSerialCheckedName()
{
    g_serialConfig.count = 0;
    g_serialConfig.checkedName = QString("");
    memset(g_serialConfig.checkedId,-1,sizeof(g_serialConfig.checkedId));

    g_serialConfig.checkedBtnList = g_pButtonGroup->buttons();
    g_serialConfig.mode = ui->serialModeBox->currentText();

    for(int i =0 ;i<g_serialConfig.checkedBtnList.length();i++)
    {
        QAbstractButton *che = g_serialConfig.checkedBtnList.at(i);
        if(che->isChecked())
        {
            g_serialConfig.checkedId[g_serialConfig.count] = g_pButtonGroup->id(che);
            g_serialConfig.count++;
            g_serialConfig.checkedName += che->text() + ",";
        }
    }
}

void serialdialog::btn_group_pressed_func(int)
{
    getSerialCheckedName();
    emit serial_config_msg(g_serialConfig);
}

void serialdialog::on_serialModeBox_currentIndexChanged(const QString &arg1)
{
    g_serialConfig.mode = ui->serialModeBox->currentText();
    emit serial_config_msg(g_serialConfig);
}

void serialdialog::on_btn_serialOk_clicked()
{
    getSerialCheckedName();
    QString str = QString(g_serialConfig.checkedName);
    QStringList list_test = str.split(",");list_test.removeAll("");
    QStringList list = g_database.tableShow("serial_port");str.clear();
    if(!list.isEmpty())
    {
        for(int i=0;i<list.size();i++)
        {
            QString str1 = list.at(i);
            for(int j=0;j<list_test.size();j++)
            {
                QString str2 = list_test.at(j);
                if(str1 == str2)
                {
                    str = QString("%1,%2").arg(str).arg(str1);
                }
            }
        }
        if(!str.isEmpty())
        {
            str.remove(0,1);
            str = QString(tr("The %1 is occupied!")).arg(str);
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr(str.toUtf8()),
                             0,this);
            mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
            mesg.setFocusPolicy(Qt::NoFocus);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(g_screenFlag == 1)
                mesg.move(g_screenWidth*2/3,g_screenHeight/3);
            else
                mesg.move(g_screenWidth/3,g_screenHeight/3);
            mesg.exec();
            return;
        }
    }
    emit serial_config_msg(g_serialConfig);
    this->close();
}

void serialdialog::on_btn_serialCancel_clicked()
{
    QAbstractButton *checkBtn;
    QList<QAbstractButton*> CheckedBtnList = g_pButtonGroup->buttons();

    for(int i = 0 ;i<CheckedBtnList.length();i++)
    {
        checkBtn = CheckedBtnList.at(i);
        checkBtn->setChecked(false);
    }

    getSerialCheckedName();
    emit serial_config_msg(g_serialConfig);
    this->close();
}

void serialdialog::on_btn_serialCheckAll_clicked()
{
    QAbstractButton *checkBtn;
    QList<QAbstractButton*> CheckedBtnList = g_pButtonGroup->buttons();

    if(ui->btn_serialCheckAll->text() == "check all")
    {
        for(int i =0 ;i<CheckedBtnList.length();i++)
        {
            checkBtn = CheckedBtnList.at(i);
            checkBtn->setChecked(true);
        }
        ui->btn_serialCheckAll->setText("check none");
    }
    else if(ui->btn_serialCheckAll->text() == "check none")
    {
        for(int i =0 ;i<CheckedBtnList.length();i++)
        {
            checkBtn = CheckedBtnList.at(i);
            checkBtn->setChecked(false);
        }
        ui->btn_serialCheckAll->setText("check all");
    }
    getSerialCheckedName();
    emit serial_config_msg(g_serialConfig);
}

void serialdialog::languageReload()
{
    ui->retranslateUi(this);
}

#if 0
void serialdialog::on_serialTestBtn_clicked()
{
    if(ui->serialTestBtn->text() == "test")
    {
        ui->serialTestEdit->clear();
        ui->serialTestBtn->setText("stop");
        ui->btn_serialOk->setDisabled(true);

        getSerialCheckedName();
        emit serial_config_msg(g_serialConfig);
        emit serial_test_msg();
    }
    else if(ui->serialTestBtn->text() == "stop")
    {
        emit serial_dialog_stop_msg();
        ui->serialTestBtn->setText("test");
        ui->btn_serialOk->setDisabled(false);
    }
}


void serialdialog::serial_set_testBt_func()
{
    ui->serialTestBtn->setText("test");
    ui->btn_serialOk->setDisabled(false);
}

void serialdialog::serial_result_recv_func(QString str)
{
    ui->serialTestEdit->appendPlainText(str);
}
#endif

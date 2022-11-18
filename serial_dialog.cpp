#include "serialdialog.h"
#include "ui_serialdialog.h"
#include <QScreen>

static int screenWidth;
static int screenHeight;
static int screenFlag;
static QScreen *screen;

serialdialog::serialdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::serialdialog)
{
    ui->setupUi(this);
    screen = qApp->primaryScreen();
    screenWidth = screen->size().width();
    screenHeight = screen->size().height();

    if(screenWidth < screenHeight)
    {
        screenFlag = 1;
    }
    QFont font;
    qreal realX = screen->physicalDotsPerInchX();
    qreal realY = screen->physicalDotsPerInchY();
    qreal realWidth = screenWidth / realX * 2.54;
    qreal realHeight = screenHeight / realY *2.54;

    if(screenFlag == 1)
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

    pButtonGroup = new QButtonGroup(this);
    pButtonGroup->setExclusive(false);               //Set to non-exclusive mode

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
        pButtonGroup->addButton(checkbox,i);
    }

    ui->groupBox->setLayout(vLayout1);
    connect(pButtonGroup,SIGNAL(buttonClicked(int)),this,SLOT(pButtonGroup_pressed_func(int)));
}

serialdialog::~serialdialog()
{
    delete ui;
}

void serialdialog::getSerialCheckedName()
{
    serialConfig.count = 0;
    serialConfig.checkedName = QString("");
    memset(serialConfig.checked_id,-1,sizeof(serialConfig.checked_id));

    serialConfig.checkedBtnList = pButtonGroup->buttons();
    serialConfig.mode = ui->serialModeBox->currentText();

    for(int i =0 ;i<serialConfig.checkedBtnList.length();i++)
    {
        QAbstractButton *che = serialConfig.checkedBtnList.at(i);
        if(che->isChecked())
        {
            serialConfig.checked_id[serialConfig.count] = pButtonGroup->id(che);
            serialConfig.count++;
            serialConfig.checkedName += che->text() + ",";
        }
    }
}

void serialdialog::pButtonGroup_pressed_func(int)
{
    getSerialCheckedName();
    emit serial_config_msg(serialConfig);
}

void serialdialog::on_serialModeBox_currentIndexChanged(const QString &arg1)
{
    serialConfig.mode = ui->serialModeBox->currentText();
    emit serial_config_msg(serialConfig);
}

void serialdialog::on_btn_serialOk_clicked()
{
    getSerialCheckedName();
    QString str = QString(serialConfig.checkedName);
    QStringList list_test = str.split(",");list_test.removeAll("");
    QStringList list = databaseWg.tableShow("serial_port");str.clear();
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
            if(screenFlag == 1)
                mesg.move(screenWidth*2/3,screenHeight/3);
            else
                mesg.move(screenWidth/3,screenHeight/3);
            mesg.exec();
            return;
        }
    }
    emit serial_config_msg(serialConfig);
    this->close();
}

void serialdialog::on_btn_serialCancel_clicked()
{
    QAbstractButton *checkBtn;
    QList<QAbstractButton*> CheckedBtnList = pButtonGroup->buttons();

    for(int i = 0 ;i<CheckedBtnList.length();i++)
    {
        checkBtn = CheckedBtnList.at(i);
        checkBtn->setChecked(false);
    }

    getSerialCheckedName();
    emit serial_config_msg(serialConfig);
    this->close();
}

void serialdialog::on_btn_serialCheckAll_clicked()
{
    QAbstractButton *checkBtn;
    QList<QAbstractButton*> CheckedBtnList = pButtonGroup->buttons();

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
    emit serial_config_msg(serialConfig);
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
        emit serial_config_msg(serialConfig);
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

#include "serialdialog.h"
#include "ui_serialdialog.h"
#include <QScreen>

static int s_width;
static int s_height;
static int screen_flag;
static QScreen *screen;

serialdialog::serialdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::serialdialog)
{
    ui->setupUi(this);
    screen = qApp->primaryScreen();
    s_width = screen->size().width();
    s_height = screen->size().height();

    if(s_width < s_height)
    {
        screen_flag = 1;
    }
    QFont font;
    qreal realX = screen->physicalDotsPerInchX();
    qreal realY = screen->physicalDotsPerInchY();
    qreal realWidth = s_width / realX * 2.54;
    qreal realHeight = s_height / realY *2.54;

    if(screen_flag == 1)
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
    ui->label->setFont(font);
    ui->label_2->setFont(font);
    ui->groupBox_3->setFont(font);
    ui->serialModeBox->setFont(font);
    ui->serialCancelBtn->setFont(font);
    ui->serialCheckAllBtn->setFont(font);
    ui->serialOkBtn->setFont(font);

    pButtonGroup = new QButtonGroup(this);
    pButtonGroup->setExclusive(false);               //设置这个按钮组为非互斥模式

    QStringList serialPortName;
    QString tmp;
    QGridLayout  *vLayout1 = new QGridLayout ;

    //查找可用的串口
    foreach(const QSerialPortInfo &Info,QSerialPortInfo::availablePorts())//读取串口信息
    {
        if(Info.portName() == QString("ttyS0"))
        {
            continue;
        }
        serialPortName << Info.portName();
        //qDebug()<<"portname: "<<Info.portName();//调试时可以看的串口信息
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

    ui->groupBox_3->setLayout(vLayout1);
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
            //qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "n:" << n;
        }
    }

}

void serialdialog::pButtonGroup_pressed_func(int)
{
//    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "button name:" << button->text();
    getSerialCheckedName();
    emit serial_config_msg(serialConfig);
}

void serialdialog::on_serialModeBox_currentIndexChanged(const QString &arg1)
{
    serialConfig.mode = ui->serialModeBox->currentText();
    emit serial_config_msg(serialConfig);
}

void serialdialog::on_serialOkBtn_clicked()
{
    getSerialCheckedName();
    QString str = QString(serialConfig.checkedName);
    QStringList list_test = str.split(",");list_test.removeAll("");
    QStringList list = database_w.table_show("serial_port");str.clear();
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
            str = QString("The %1 is occupied!").arg(str);
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr(str.toUtf8()),
                             0,this);
            mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
            mesg.setFocusPolicy(Qt::NoFocus);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(screen_flag == 1)
                mesg.move(s_width*2/3,s_height/3);
            else
                mesg.move(s_width/3,s_height/3);
            mesg.exec();
            return;
        }
    }
    emit serial_config_msg(serialConfig);
    this->close();
}

void serialdialog::on_serialCancelBtn_clicked()
{
    QAbstractButton *checkBtn;
    QList<QAbstractButton*> CheckedBtnList = pButtonGroup->buttons();

//    if(ui->serialTestBtn->text() == "stop")
//    {
//        emit serial_dialog_stop_msg();
//        ui->serialTestBtn->setText("test");
//        ui->serialOkBtn->setDisabled(false);
//    }

    for(int i = 0 ;i<CheckedBtnList.length();i++)
    {
        checkBtn = CheckedBtnList.at(i);
        checkBtn->setChecked(false);
    }

    getSerialCheckedName();
    emit serial_config_msg(serialConfig);

    this->close();
}

void serialdialog::on_serialCheckAllBtn_clicked()
{
    QAbstractButton *checkBtn;
    QList<QAbstractButton*> CheckedBtnList = pButtonGroup->buttons();

    if(ui->serialCheckAllBtn->text() == "check all")
    {
        for(int i =0 ;i<CheckedBtnList.length();i++)
        {
            checkBtn = CheckedBtnList.at(i);
            checkBtn->setChecked(true);
        }
        ui->serialCheckAllBtn->setText("check none");
    }
    else if(ui->serialCheckAllBtn->text() == "check none")
    {
        for(int i =0 ;i<CheckedBtnList.length();i++)
        {
            checkBtn = CheckedBtnList.at(i);
            checkBtn->setChecked(false);
        }
        ui->serialCheckAllBtn->setText("check all");
    }
    getSerialCheckedName();
    emit serial_config_msg(serialConfig);
}

void serialdialog::language_reload()
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
        ui->serialOkBtn->setDisabled(true);

        getSerialCheckedName();
        emit serial_config_msg(serialConfig);
        emit serial_test_msg();
    }
    else if(ui->serialTestBtn->text() == "stop")
    {
        emit serial_dialog_stop_msg();
        ui->serialTestBtn->setText("test");
        ui->serialOkBtn->setDisabled(false);
    }
}


void serialdialog::serial_set_testBt_func()
{
    ui->serialTestBtn->setText("test");
    ui->serialOkBtn->setDisabled(false);
}

void serialdialog::serial_result_recv_func(QString str)
{
    ui->serialTestEdit->appendPlainText(str);
}
#endif

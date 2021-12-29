#include "serialdialog.h"
#include "ui_serialdialog.h"

serialdialog::serialdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::serialdialog)
{
    ui->setupUi(this);

    pButtonGroup = new QButtonGroup(this);
    pButtonGroup->setExclusive(false);               //设置这个按钮组为非互斥模式

    //pButtonGroup->addButton(ui->btnOther, 3);

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

void serialdialog::on_serialOkBtn_clicked()
{
//    getSerialCheckedName();
//    emit serial_config_msg(serialConfig);
    this->close();
}

void serialdialog::on_serialCancelBtn_clicked()
{
    if(ui->serialModeBox->currentText() == "server" && ui->serialTestBtn->text() == "stop")
    {
        return;
    }
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

void serialdialog::on_serialTestBtn_clicked()
{
    if(ui->serialTestBtn->text() == "test")
    {
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

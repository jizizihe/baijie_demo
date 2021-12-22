#include "serialdialog.h"
#include "ui_serialdialog.h"

serialdialog::serialdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::serialdialog)
{
    ui->setupUi(this);

    pButtonGroup = new QButtonGroup(this);
    pButtonGroup->setExclusive(false);               //设置这个按钮组为非互斥模式

    ui->serialPortListWidget->hide();

    //pButtonGroup->addButton(ui->btnOther, 3);

    QStringList serialPortName;
    QString tmp;
    QGridLayout  *vLayout1 = new QGridLayout ;

    //查找可用的串口
    foreach(const QSerialPortInfo &Info,QSerialPortInfo::availablePorts())//读取串口信息
    {
        serialPortName << Info.portName();
        qDebug()<<"portname: "<<Info.portName();//调试时可以看的串口信息
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

}

serialdialog::~serialdialog()
{
    delete ui;
}

QString serialdialog::getSerialCheckedName()
{
    serialConfig.count = 0;
    serialConfig.checkedName = QString("");
    memset(serialConfig.checked_id,-1,sizeof(serialConfig.checked_id));

    int n;
    serialConfig.checkedBtnList = pButtonGroup->buttons();

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
    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "name:" << serialConfig.checkedName;

    return serialConfig.checkedName;
}


void serialdialog::on_serialOkBtn_clicked()
{
    getSerialCheckedName();
    serialConfig.mode = ui->serialModeBox->currentText();
    emit serial_config_msg(serialConfig);

    this->close();
}

void serialdialog::on_serialCancelBtn_clicked()
{
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
}


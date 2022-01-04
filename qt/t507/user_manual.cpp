#include "user_manual.h"
#include "ui_user_manual.h"
#include <QDebug>
#include <QString>
user_manual::user_manual(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::user_manual)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground,true);

    ui->textEdit->setReadOnly(true);
    ui->treeWidget->verticalScrollBar()->setStyleSheet("QScrollBar{width:25px;}");
    ui->textEdit->verticalScrollBar()->setStyleSheet("QScrollBar{width:25px;}");

}

user_manual::~user_manual()
{
    delete ui;
}

void user_manual::on_pushButton_clicked()
{
    emit Mysignal();
    ui->textEdit->clear();
    ui->pushButton_2->setText(tr("expand all"));
    catalogueflag = true;
    ui->treeWidget->collapseAll();
}

void user_manual::language_reload()
{
    ui->retranslateUi(this);
}


void user_manual::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    QString str = item->text(column);

    if(str == tr("wifi status"))
    {
        ui->textEdit->setText(tr("You can use the < wifi status > button to view the current connection status, and on this page you can choose to modify the password or remove the connection."));
    }
    else if(str == tr("wifi connect"))
    {
        ui->textEdit->setText(tr("When you click the < wifi connect > button, a list of connectable WiFi will appear. Click the WiFi you want to connect, and a connection dialog box will pop up for those who have not been connected before. For those who have been connected before, the connection will be activated and jump to the WiFi status interface. Switch to control the WiFi on and off, and click the < refresh > button to update the WiFi list."));
    }
    else if(str == tr("hotspot connect"))
    {
        ui->textEdit->setText(tr("When you click the < hotspot connect > button, you can choose to establish a hotspot connection or disconnect."));
    }
    else if(str == tr("bluetooth scan"))
    {
        ui->textEdit->setText(tr("When you click the < Bluetooth scan > button, scan the surrounding Bluetooth devices."));
    }
    else if(str == tr("bluetooth pair"))
    {
        ui->textEdit->setText(tr("When you select a Bluetooth, click the < Bluetooth pair > button to pair."));
    }
    else if(str == tr("bluetooth connect"))
    {
        ui->textEdit->setText(tr("When you select a Bluetooth, click the < Bluetooth connect > button to connect."));
    }
    else if(str == tr("show all ip connect"))
    {
        ui->textEdit->setText(tr("You can use < show all IP addresses > to view the IP addresses of all network configurations, which is equivalent to 'ifconfig'. \
You can use < show all connections > to view all current network connections."));
    }
    else if(str == tr("set dynamic ip "))
    {
        ui->textEdit->setText(tr("Click the < set dynamic IP > button, enter the network device you want to set as dynamic IP, and the setting result will be displayed in the prompt box."));
    }
    else if(str == tr("set static ip"))
    {
        ui->textEdit->setText(tr("You can add, delete and modify static connections, and you can view the connections you just added through the < show all connections > button."));
    }
    else if(str == tr("network speed test"))
    {
        ui->textEdit->setText(tr("You can check the network speed in real time through < network speed test >.However,if you select other buttons or prompt boxes, the network speed test will stop."));
    }
    else if(str == tr("RTC"))
    {
        ui->textEdit->setText(tr("You can use the < systimeSet > button to set the system time, or you can use the < RTC write > button to synchronize to the hardware time."));
    }
    else if(str == tr("brightness"))
    {
        ui->textEdit->setText(tr("You can adjust the brightness of the screen through the slider."));
    }
    else if(str == tr("Sleep time"))
    {
        ui->textEdit->setText(tr("You can adjust the sleep time yourself."));
    }
    else if(str == tr("scan occupied gpio"))
    {
        ui->textEdit->setText(tr("You can view the currently occupied GPIO port by pressing the < scan > button."));
    }
    else if(str == tr("batch operation"))
    {
        ui->textEdit->setText(tr("You can configure the input / output of GPIO port and pull it up / down when it is set to output mode.\n \
You can also operate on multiple GPIO ports, but each two GPIO ports should be separated by ','. \n \
Note: there should be no space between each GPIO air, otherwise this operation will fail."));
    }
    else if(str == tr("Serial Port"))
    {
        ui->textEdit->setText(tr("First, make sure that the hardware connection corresponds, then set the serial port properties, and click the < open / close > button to open / close the serial port. Enter the content you want to send in the input box, click the < send > button to send, and the receiver will receive the information."));
    }
    else if(str == tr("choose file"))
    {
        ui->textEdit->setText(tr("Note: the newly created folder should not contain spaces, otherwise the new recording file cannot be saved."));
    }
    else if(str == tr("begin"))
    {
        ui->textEdit->setText(tr("You can press the < start > key to record. The next time you press this key, end the recording and save it in '/ data' or the file directory you choose."));
    }
    else if(str == tr("play"))
    {
        ui->textEdit->setText(tr("You can adjust the volume when playing the recording."));
    }
    else if(str == tr("Touch Display"))
    {
        ui->textEdit->setText(tr("This function will recognize the touch screen and display the movement track on the screen."));
    }
    else if(str == tr("udev"))
    {
        ui->textEdit->setText(tr("You can operate files from external storage devices."));
    }
    else if(str == tr("SD"))
    {
        ui->textEdit->setText(tr("You can test whether there is SD insertion through this function."));
    }
    else if(str == tr("SIM"))
    {
        ui->textEdit->setText(tr("This function should not be clicked too often. After the previous test is displayed, it will be tested according to your needs. Pay special attention to check whether the SIM card is inserted if a prompt box appears when detecting the SIM card. If it is not inserted, please do not continue to click. Because the current operation will be delayed, it will have a bad sense of experience. During the waiting process, it is necessary to pay attention to the change of 4G LED lamp. When the card is detected or pulled out, the LED lamp will flash - after detecting the flash of card insertion, it will enter the normally on state and pause for 500ms; When it is detected that the card is pulled out, after the flash, enter the pause (on for 500ms)."));
    }
    else if(str == tr("All Interface Test"))
    {
        ui->textEdit->setText(tr("First, check the item to be tested. For USB test, select the number of external USB (0 by default), select the serial port number (none of the serial ports are selected by default), and then click the < begin > button to start the test. The results are displayed on the right."));
    }
    else if(str == tr("Chinese/English"))
    {
        ui->textEdit->setText(tr("Support Chinese / English switching."));
    }

}

void user_manual::on_pushButton_2_clicked()
{
    if(catalogueflag)
    {
        ui->treeWidget->expandAll();
        ui->pushButton_2->setText(tr("collapse all"));
    }
    else
    {
        ui->treeWidget->collapseAll();
        ui->pushButton_2->setText(tr("expand all"));
    }
    catalogueflag = !catalogueflag;
}



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
    if(str == tr("wifi connect"))
    {
        ui->textEdit->setText(tr("You can use the < WiFi connection > key to scan the surrounding WiFi signals and enter the password to connect. Or use and you can use the < signal quality > and < connection status > keys to view the signal strength and WiFi connection of the currently connected WiFi."));
    }
    else if(str == tr("hot spot"))
    {
        ui->textEdit->setText(tr("You can use the < hotspot > button to turn on or off the hotspot of the board."));
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
        ui->textEdit->setText(tr("You can use the < systimesetbt > button to set the system time, or you can use < rtcsetbt > to set the hardware time."));
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
    else if(str == tr("test"))
    {
        ui->textEdit->setText(tr("Press the < test > button to test all interfaces, the test results will be displayed in the prompt box, and the photos taken by the camera will be displayed in the lower left corner."));
    }
    else if(str == tr("photo"))
    {
        ui->textEdit->setText(tr("You can press the < photo > button to take a picture again, but you need to press the < display > button to take a good picture before it can be loaded again."));
    }
    else if(str == tr("Key Test"))
    {
        ui->textEdit->setText(tr("You can use this function to test the buttons on the board. After pressing the < start > button, when you press the button on the board, a prompt will be displayed in the prompt box, and then press the < stop > button to end the test"));
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



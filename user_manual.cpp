#include "user_manual.h"
#include "ui_user_manual.h"
#include <QDebug>
#include <QString>
#include <QScreen>

static int s_width;
static int s_height;
static QScreen *screen;
static int screen_flag;

user_manual::user_manual(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::user_manual)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground,true);

    ui->textEdit->setReadOnly(true);
    ui->treeWidget->verticalScrollBar()->setStyleSheet("QScrollBar{width:25px;}");
    ui->textEdit->verticalScrollBar()->setStyleSheet("QScrollBar{width:25px;}");
    screen = qApp->primaryScreen();
    s_width = screen->size().width();
    s_height = screen->size().height();
    user_font();
}

user_manual::~user_manual()
{
    delete ui;
}

void user_manual::on_pushButton_clicked()
{
    emit Mysignal();
    ui->textEdit->clear();
    ui->pushButton_3->setText(tr("expand all"));
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

    if(str == tr("wifi and hotspot status"))
    {
        ui->textEdit->setText(tr("You can use the the switch button control the WiFi on and off. You can click the < status > button to view the WiFi current connection status and hotspot status, and on Wifi Status page you can choose to modify the password or remove the connection."));
    }
    else if(str == tr("wifi connect"))
    {
        ui->textEdit->setText(tr("When you click the < wifi > button, a list of connectable WiFi will appear,and the list is refreshed every 5 seconds. Click the WiFi you want to connect, and a connection dialog box will pop up for those who have not been connected before.You can enter the password in the dialog box to connect. For those who have been connected before, the connection will be activated and jump to the WiFi status interface."));
    }
    else if(str == tr("hotspot connect"))
    {
        ui->textEdit->setText(tr("When you click the < hotspot > button, you can choose to establish a hotspot connection or disconnect."));
    }
    else if(str == tr("bluetooth scan"))
    {
        ui->textEdit->setText(tr("When you click the < scan > button, scan the surrounding Bluetooth devices."));
    }
    else if(str == tr("bluetooth pair"))
    {
        ui->textEdit->setText(tr("You can select a Bluetooth in scan devices, click the < pair > button to pair."));
    }
    else if(str == tr("bluetooth connect"))
    {
        ui->textEdit->setText(tr("You can select a Bluetooth in pair devices, click the < connect > button to connect."));
    }
    else if(str == tr("ip information"))
    {
        ui->textEdit->setText(tr("You can use < ip information> to view the IP addresses of all network configurations, which is equivalent to 'ifconfig'."));
    }
    else if(str == tr("set static ip "))
    {
        ui->textEdit->setText(tr("You can set a static IP by clicking the < set static ip > button and entering the ip you want to set."));
    }
    else if(str == tr("mod static ip"))
    {
        ui->textEdit->setText(tr("You can modify a static IP by clicking the < mod static ip > button and entering the ip you want to modify."));
    }
    else if(str == tr("set auto ip"))
    {
        ui->textEdit->setText(tr("When you click the < set auto ip > button, the set static ip will be removed and the network will become a dynamic IP."));
    }

    else if(str == tr("RTC"))
    {
        ui->textEdit->setText(tr("You can use the < systimeSet > button to set the system time, or you can use the < RTCSet > button to synchronize to the hardware time."));
    }
    else if(str == tr("brightness"))
    {
        ui->textEdit->setText(tr("You can adjust the brightness of the screen through the slider."));
    }
    else if(str == tr("Sleep time"))
    {
        ui->textEdit->setText(tr("You can adjust the sleep time yourself."));
    }
    else if(str == tr("Using tips"))
    {
        ui->textEdit->setText(tr("For example: If you want to check GPIOG13 ,please input 'g13'"));
    }
    else if(str == tr("batch operation"))
    {
        ui->textEdit->setText(tr("You can configure the input / output of GPIO port and pull it up / down when it is set to output mode.\n \
You can also operate on multiple GPIO ports, but each two GPIO ports should be separated by ','. \n \
Note: there should be no space between each GPIO air, otherwise this operation will fail."));
    }
    else if(str == tr("Serial Port"))
    {
        ui->textEdit->setText(tr("First, make sure that the hardware connection corresponds, then click the set serial port button to set the serial port properties.  Enter the content you want to send in the input box, click the < send > button to send, and the receiver will receive the information."));
    }
    else if(str == tr("record"))
    {
        ui->textEdit->setText(tr("You can click the < start > button to record. The next time you press this button, end the recording and save it in '/ data' or the file directory you choose,and you can select the default name for saving recordings or change the name in the input box.Note: the newly created folder should not contain spaces, otherwise the new recording file cannot be saved."));
    }
    else if(str == tr("play"))
    {
        ui->textEdit->setText(tr("You can click the < Play > button to switch to the playback screen, select the path where the recording you want to play is located, click the < Play > button to play, slide the volume bar to adjust the volume, click the < Rename > button to rename the recording file, and click the < Delete > button to delete the recording file."));
    }
    else if(str == tr("Touch Display"))
    {
        ui->textEdit->setText(tr("This function will recognize the touch screen and display the movement track on the screen."));
    }
    else if(str == tr("copy"))
    {
        ui->textEdit->setText(tr("Select the file that you want to copy in the directory, you can select multiple files. Click the < copy > button to copy, and then select the directory you want to paste and click the < Paste > button to paste or click the < cancel > to cancel copy."));
    }
    else if(str == tr("cut"))
    {
        ui->textEdit->setText(tr("Select the file that you want to cut in the directory, you can select multiple files. Click the < cut > button to cut, and then select the directory you want to paste and click the < Paste > button to cut or click the < cancel > cancel to cut."));
    }
    else if(str == tr("delete"))
    {
        ui->textEdit->setText(tr("Select the file that you want to delete in the directory, you can select multiple files. Click the < delete > button to delete."));
    }
    else if(str == tr("safe exit"))
    {
        ui->textEdit->setText(tr("You can click the < USB flash disk/sdcard > to jump to the appropriate directory, select the file that you want to unmount. Click the < safe exit > button to unmount."));
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

void user_manual::user_font()
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
  ui->treeWidget->setFont(font);
  ui->pushButton_3->setFont(font);
  ui->textEdit->setFont(font);
}

void user_manual::on_pushButton_3_clicked()
{
    if(catalogueflag)
        {
            ui->treeWidget->expandAll();
            ui->pushButton_3->setText(tr("collapse all"));
        }
        else
        {
            ui->treeWidget->collapseAll();
            ui->pushButton_3->setText(tr("expand all"));
        }
        catalogueflag = !catalogueflag;
}

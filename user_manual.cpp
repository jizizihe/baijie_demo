#include "user_manual.h"
#include "ui_user_manual.h"
#include <QDebug>
#include <QString>
#include <QScreen>

static int screenWidth;
static int screenHeight;
static int screenFlag;
static QScreen *screen;

user_manual::user_manual(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::user_manual)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground,true);

    ui->textEdit->setReadOnly(true);
    ui->treeWidget->verticalScrollBar()->setStyleSheet("QScrollBar{width:25px;}");
    ui->textEdit->verticalScrollBar()->setStyleSheet("QScrollBar{width:25px;}");
    ui->textEdit->setWordWrapMode(QTextOption::WrapAnywhere);
    screen = qApp->primaryScreen();
    screenWidth = screen->size().width();
    screenHeight = screen->size().height();
    if(screenWidth < screenHeight)
    {
        screenFlag = 1;
        ui->line->setStyleSheet("background-color: rgb(186, 189, 182);");
    }
    userFont();
}

user_manual::~user_manual()
{
    delete ui;
}

void user_manual::on_btn_ret_clicked()
{
    emit user_manual_back_msg();
    ui->textEdit->clear();
    ui->btn_ecpandAll->setText(tr("expand all"));
    btnExpandAllFlag = true;
    ui->treeWidget->collapseAll();
}

void user_manual::languageReload()
{
    ui->retranslateUi(this);
}

void user_manual::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    QString str = item->text(column);

    if(str == tr("WiFi"))
    {
        ui->textEdit->setText(tr("    You can use the the switch button in the upper right control the wifi on and off."));
    }
    else if(str == tr("connect WiFi"))
    {
        ui->textEdit->setText(tr("    You can click the < scan > button to dispaly a list of  connected, saved, and scanned WiFi devices, "
                                 "and the list is refreshed every 10 seconds.\n    Click the WiFi you want to connect, and a connection "
                                 "dialog box will pop up for those who have not been saved before.You can enter the password in the "
                                 "dialog box to connect.For those who have been saved before, the click will be connected and jump to "
                                 "the WiFi status interface.\n    You can click the < change passwd > button to change password and click the < remove > "
                                 "button to delect WiFi after jump to  the WiFi status interface."));
    }
    else if(str == tr("connect hotspot"))
    {
        ui->textEdit->setText(tr("    When you click the < hotspot > button, you can choose to establish a hotspot connection or disconnect. "
                                 "After connecting the hotspot, click < change > button to modify the hotspot."));
    }
    else if(str == tr("Bluetooth"))
    {
        ui->textEdit->setText(tr("    You can use the the switch button in the upper right control the bluetooth on and off.The bluetooth list shows connected, "
                                 "saved, and scanned bluetooth devices. The list is scanned every 10 seconds, or you can click the < scan > button to scan.\n"
                                 "    You can click the connected bluetooth to view the bluetooth information, click the saved bluetooth to connect, "
                                 "and click  scan bluetooth to pair.When viewing the connected bluetooth information, you can click the button to disconnect or delete the bluetooth."));
    }
    else if(str == tr("Ethernet"))
    {
        ui->textEdit->setText(tr("    You can use the the switch button in the upper right control the Ethernet on and off."));
    }
    else if(str == tr("ip information"))
    {
        ui->textEdit->setText(tr("    You can use < IP info > to view the IP addresses of all network configurations, which is equivalent to 'ifconfig'."));
    }
    else if(str == tr("ip set"))
    {
        ui->textEdit->setText(tr("    You can set a static IP by clicking the < set static IP > button and entering the ip you want to set."
                                 "You can modify a static IP by clicking the < change > button and entering the ip you want to modify after you set static IP.\n"
                                 "    When you click the < set auto ip > button, the set static ip will be removed and the network will become a dynamic IP."));
    }
    else if(str == tr("4G"))
    {
        ui->textEdit->setText(tr("    You can use the the switch button in the upper right control the 4G on and off.Click the < status > button to view the status "
                                 "information of the module, click the < connect > button to connect to 4G, "
                                 "and click the < disconnect > button to disconnect the 4G."));
    }
    else if(str == tr("RTC"))
    {
        ui->textEdit->setText(tr("    You can use the < sync network > button to set the system time to synchronize with the network time, "
                                 "and click the button again to unsync.\n    You can use the < systimeset > button to set the system time, "
                                 "or you can use the < RTCset > button to synchronize to the hardware time."));
    }
    else if(str == tr("backlight"))
    {
        ui->textEdit->setText(tr("    You can adjust the backlight of the screen through the slider."));
    }
    else if(str == tr("sleep time"))
    {
        ui->textEdit->setText(tr("    You can click the < sleep > button and select the sleep time to set."));
    }
    else if(str == tr("OTG"))
    {
        ui->textEdit->setText(tr("    You can click the Set button to switch between the slave and host devices."));
    }
    else if(str == tr("GPIO"))
    {
        ui->textEdit->setText(tr("    You can configure the input / output of GPIO port and pull it up / down when it is set to output mode.\n"
                                 "    You can also operate on multiple GPIO ports, but each two GPIO ports should be separated by ','. \n"
                                 "    Note: there should be no space between each GPIO air, otherwise this operation will fail."));
    }
    else if(str == tr("Serial Port"))
    {
        ui->textEdit->setText(tr("    First, make sure that the hardware connection corresponds, then Click the < set  > button to configure the serial port properties, click the serial port button switch.\n"
                                 "    Enter the content you want to send in the input box, click the < send > button to send, and the receiver will receive the information."));
    }
    else if(str == tr("record"))
    {
        ui->textEdit->setText(tr("    You can click the < start > button to record. The next time you press this button, end the recording and save "
                                 "it in '/ data' or the file directory you choose,and you can select the default name for saving recordings or change "
                                 "the name in the input box.\n    Note: the newly created folder should not contain spaces, otherwise the new recording "
                                 "file cannot be saved."));
    }
    else if(str == tr("play"))
    {
        ui->textEdit->setText(tr("    You can click the < play > button to switch to the playback screen, select the path where the recording you want "
                                 "to play is located, click the < Play > button to play, slide the volume bar to adjust the volume.\n    click the < rename > "
                                 "button to rename the recording file, and click the < delete > button to delete the recording file."));
    }
    else if(str == tr("Touch Display"))
    {
        ui->textEdit->setText(tr("    This function will recognize the touch screen and display the movement track on the screen."));
    }
    else if(str == tr("copy"))
    {
        ui->textEdit->setText(tr("    Select the file that you want to copy in the directory, you can select multiple files. Click the < copy > button "
                                 "to copy, and then select the directory you want to paste and click the < paste > button to paste."));
    }
    else if(str == tr("cut"))
    {
        ui->textEdit->setText(tr("    Select the file that you want to cut in the directory, you can select multiple files. Click the < cut > button to cut, "
                                 "and then select the directory you want to paste and click the < paste > button to cut."));
    }
    else if(str == tr("delete"))
    {
        ui->textEdit->setText(tr("    Select the file that you want to delete in the directory, you can select multiple files. Click the < delete > button to delete."));
    }
    else if(str == tr("safe exit"))
    {
        ui->textEdit->setText(tr("    You can click the < Udisk/sdcard > to jump to the appropriate directory, select the file that you want to safe unplug. "
                                 "Click the < safe unplug > button to safe unplug."));
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
        ui->textEdit->setText(tr("    First, check the item to be tested. For USB test, select the number of external USB (0 by default), select the serial "
                                 "port number (none of the serial ports are selected by default), and then click the < begin > button to start the test. The results are displayed on the right."));
    }
    else if(str == tr("Chinese/English"))
    {
        ui->textEdit->setText(tr("    Click < Chinese/English > button to switch between Chinese and English."));
    }
}

void user_manual::userFont()
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
    ui->treeWidget->setFont(font);
    ui->btn_ecpandAll->setFont(font);
    ui->textEdit->setFont(font);
    ui->lbl_userManual->setFont(font);
}

void user_manual::on_btn_ecpandAll_clicked()
{
    if(btnExpandAllFlag)
    {
        ui->treeWidget->expandAll();
        ui->btn_ecpandAll->setText(tr("collapse all"));
    }
    else
    {
        ui->treeWidget->collapseAll();
        ui->btn_ecpandAll->setText(tr("expand all"));
    }
    btnExpandAllFlag = !btnExpandAllFlag;
}

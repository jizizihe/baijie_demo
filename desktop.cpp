#include "desktop.h"
#include "ui_desktop.h"
#include <QScreen>
#include <QTimer>

static int g_screenWidth;
static int g_screenHeight;

Desktop::Desktop(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Desktop)
{
    ui->setupUi(this);
    QScreen *screen = qApp->primaryScreen();
    g_screenWidth = screen->size().width();
    g_screenHeight = screen->size().height();
}

Desktop::~Desktop()
{
    delete ui;
}

void Desktop::mousePressEvent(QMouseEvent *event)
{
    this->hide();
    g_mainwindowWg.resize(g_screenWidth,g_screenHeight);
    g_mainwindowWg.show();
}

void Desktop::select_language()
{
    ui->lbl_img_name->setText("微信二维码");
    ui->lbl_tel->setText("业务电话:13510178868");
    ui->btn_img->setIcon(QIcon(":/button_image/weixin.jpg"));
    ui->lbl_title_1->setText("百杰开发板演示系统");
    ui->lbl_title_2->setVisible(false);
    ui->lbl_tips->setText("点击任意处进入演示");
    ui->lbl_address_2->setText("地址:深圳市宝安区西乡街道宝源路宝安互联网产业基地 A 区5栋306");
    ui->lbl_sopport->setText("支持软硬件定制服务");
    ui->widget_3->setMaximumSize(0,0);
    ui->widget_5->layout()->setSpacing(10);
    ui->btn_img->setIconSize(QSize(150,150));
    emit language_translate();
    QFont font;
    font.setLetterSpacing(QFont::PercentageSpacing,110);         //word space
    font.setPointSize(21);
    font.setBold(true);
    font.setItalic(true);
    ui->lbl_title_1->setFont(font);
}

void Desktop::showEvent(QShowEvent *event)
{
    QStringList list = g_database.tableShow("language");
    if(!list.isEmpty())
    {
        if(list.at(0) == "Chinese")
        {
            select_language();
        }
    }
}

QString Desktop::executeLinuxCmd(QString strCmd)
{
    QProcess pro;
    pro.start("bash", QStringList() <<"-c" << strCmd);
    pro.waitForFinished(-1);
    QString strResult = pro.readAllStandardOutput();qDebug() << strResult;
    strResult = pro.readAllStandardError();qDebug() << strResult;
    pro.close();
    return strResult;
}

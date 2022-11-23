#include "language_selection.h"
#include "ui_language_selection.h"
#include <QScreen>
#include <stdlib.h>

static int g_screenWidth;
static int g_screenHeight;

language_selection::language_selection(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::language_selection)
{
    ui->setupUi(this);
    ui->rtn_English->setChecked(true);
    setLanguageFont();
}

language_selection::~language_selection()
{
    delete ui;
}

void language_selection::setLanguageFont()
{
    QScreen *screen = qApp->primaryScreen();
    g_screenWidth = screen->size().width();
    g_screenHeight = screen->size().height();
    qreal realX = screen->physicalDotsPerInchX();
    qreal realWidth = g_screenWidth / realX * 2.54;
    QFont font;

    if(realWidth < 15)
    {
        font.setPointSize(12);
        ui->lbl_tips->setFont(font);
        font.setPointSize(13);
    }
    else if (realWidth < 18)
    {
        font.setPointSize(15);
        ui->lbl_tips->setFont(font);
        font.setPointSize(16);
    }
    else
    {
        font.setPointSize(18);
        ui->lbl_tips->setFont(font);
        font.setPointSize(19);
    }
    ui->rtn_Chinese->setFont(font);
    ui->rtn_English->setFont(font);
}

void language_selection::on_btn_ok_clicked()
{
    QStringList list = g_database.tableShow("language");
    if(list.isEmpty())
    {
       g_database.insertTableOne("language","English");
    }
    if(ui->rtn_Chinese->isChecked())
    {
        g_database.updateTableOne("language","Chinese");
    }

    this->hide();
    g_desktopWg.resize(g_screenWidth,g_screenHeight);
    g_desktopWg.show();
}

void language_selection::desktopShow()
{
    QStringList list = g_database.tableShow("language");
    if(list.at(0) == "Chinese")
    {
        ui->rtn_Chinese->setCheckable(true);
    }
    this->hide();
    g_desktopWg.resize(g_screenWidth,g_screenHeight);
    g_desktopWg.show();
}
